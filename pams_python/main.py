import time

import numpy as np

from silhouette import create_pam_clusters


def main():
    dist_matrix_file = "..\\dist_matrix(10000x10000).txt"

    try:
        dist_matrix = np.loadtxt(dist_matrix_file, delimiter=',')
    except IOError:
        print("Unable to open file")
        return
    print("Parsing distance matrix is finished")
    print("Start clustering")
    start_time = time.time()
    clusters = create_pam_clusters(dist_matrix)
    elapsed_time = time.time() - start_time

    print(f"\n\nElapsed time:\t {elapsed_time:.6f} seconds\n")

    print("clusters = {")
    for cluster_id, cluster in enumerate(clusters):
        print(f"    {cluster_id}: [{', '.join(map(str, cluster))}],")
    print("}")
    print(f"Number of Clusters: {len(clusters)}")


if __name__ == "__main__":
    main()