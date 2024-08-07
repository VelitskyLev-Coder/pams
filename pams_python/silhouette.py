import numpy as np
from pam_builder import PamBuilder


def calc_silhouette(dist_matrix, clusters):
    num_points = dist_matrix.shape[0]
    silhouette_vals = np.zeros(num_points)

    for i, cluster in enumerate(clusters):
        for point in cluster:
            a = np.mean([dist_matrix[point, other] for other in cluster if other != point])
            b = np.min(
                [np.mean([dist_matrix[point, other] for other in clusters[j]]) for j in range(len(clusters)) if j != i])
            silhouette_vals[point] = (b - a) / max(a, b)

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
