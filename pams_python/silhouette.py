import numpy as np
from pam_builder import PamBuilder


def calc_silhouette(dist_matrix, clusters):
    num_points = dist_matrix.shape[0]
    a_vals = np.zeros(num_points)
    b_vals = np.full(num_points, np.inf)

    point_indices = np.arange(num_points)

    for i, cluster in enumerate(clusters):
        in_cluster = np.isin(point_indices, cluster)
        cluster_dist = dist_matrix[np.ix_(in_cluster, in_cluster)]
        a_vals[in_cluster] = np.mean(np.ma.masked_where(cluster_dist == np.inf, cluster_dist), axis=1)

        # Calculate b_i values by comparing to other clusters
        for j, other_cluster in enumerate(clusters):
            if i != j:
                # Boolean mask for the points in the other cluster
                other_in_cluster = np.isin(point_indices, other_cluster)
                # Calculate mean distance from points in current cluster to points in other cluster
                inter_cluster_dist = np.mean(dist_matrix[np.ix_(in_cluster, other_in_cluster)], axis=1)
                # Update b_i values only if the calculated mean is lower than the current b_i
                b_vals[in_cluster] = np.minimum(b_vals[in_cluster], inter_cluster_dist)

    # Calculate the silhouette values
    silhouette_vals = (b_vals - a_vals) / np.maximum(a_vals, b_vals)

    # Calculate the overall mean silhouette score
    silhouette_score = np.mean(silhouette_vals)

    return silhouette_score


def create_pam_clusters(dist_matrix):
    DECREASE_THRESHOLD = 5

    best_clusters = []
    best_silhouette_value = -1
    prev_silhouette_value = -np.inf
    decrease_count = 0

    pam_builder = PamBuilder(dist_matrix)

    for k in range(2, dist_matrix.shape[0] + 1):
        clusters, medoids = pam_builder.pam(k)
        silhouette_value = calc_silhouette(dist_matrix, clusters)
        print(f"Silhouette for k={k}: {silhouette_value}")

        if silhouette_value > best_silhouette_value:
            best_silhouette_value = silhouette_value
            best_clusters = clusters

        if silhouette_value < prev_silhouette_value:
            decrease_count += 1
        else:
            decrease_count = 0

        if decrease_count == DECREASE_THRESHOLD:
            print(f"Stopping at k={k} due to {DECREASE_THRESHOLD} consecutive decreases in silhouette value.")
            break

        prev_silhouette_value = silhouette_value

    return best_clusters
