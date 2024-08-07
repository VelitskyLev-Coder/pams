import numpy as np


class PamBuilder:
    def __init__(self, dist_matrix):
        self.distance_matrix = dist_matrix
        self.initialize_medoid_result_cache = []
        self.initialize_medoid_min_distance_cache = []

    def initialize_medoids(self, k):
        if len(self.initialize_medoid_result_cache) >= k:
            return self.initialize_medoid_result_cache[:k]

        num_points = self.distance_matrix.shape[0]
        remaining_points = set(range(num_points))

        if not self.initialize_medoid_result_cache:
            first_medoid = np.argmin(self.distance_matrix.sum(axis=1))
            self.initialize_medoid_result_cache.append(first_medoid)
            remaining_points.remove(first_medoid)
            self.initialize_medoid_min_distance_cache = self.distance_matrix[first_medoid].copy()

        while len(self.initialize_medoid_result_cache) < k:
            gains = np.zeros(num_points)
            for i in remaining_points:
                gains[i] = np.maximum(self.initialize_medoid_min_distance_cache - self.distance_matrix[i], 0).sum()
            best_candidate = np.argmax(gains)
            self.initialize_medoid_result_cache.append(best_candidate)
            remaining_points.remove(int(best_candidate))
            self.initialize_medoid_min_distance_cache = np.minimum(self.initialize_medoid_min_distance_cache, self.distance_matrix[best_candidate])

        return self.initialize_medoid_result_cache[:k]

    def assign_points_to_medoids(self, medoids):
        num_points = self.distance_matrix.shape[0]
        assigned_indices = np.argmin(self.distance_matrix[:, medoids], axis=1)
        assigned_medoids = [medoids[index] for index in assigned_indices]
        return assigned_medoids

    def get_best_medoid_index_at_cluster(self, clusters, cluster_medoid_index):
        cluster_points = np.where(clusters == cluster_medoid_index)[0]
        if len(cluster_points) == 0:
            print(f"No points found for cluster medoid index {cluster_medoid_index}")
            return cluster_medoid_index
        distances_sum = self.distance_matrix[cluster_points][:, cluster_points].sum(axis=1)
        best_medoid = cluster_points[np.argmin(distances_sum)]
        return best_medoid

    def pam(self, k):
        medoids = self.initialize_medoids(k)
        clusters = self.assign_points_to_medoids(medoids)

        while True:
            improved = False
            for i, medoid in enumerate(medoids):
                best_new_medoid = self.get_best_medoid_index_at_cluster(clusters, medoid)
                if best_new_medoid != medoid:
                    medoids[i] = best_new_medoid
                    improved = True
            if not improved:
                break
            clusters = self.assign_points_to_medoids(medoids)

        result_clusters = [np.where(clusters == medoid)[0].tolist() for medoid in medoids]
        return result_clusters, medoids
