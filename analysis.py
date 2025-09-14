import sys
import numpy as np
from sklearn.metrics import silhouette_score
from symnmf import get_matrix_by_goal
from kmeans import get_kmeans_centroids, get_argmin_k

max_iter = 300
eps = 1e-4 

def get_symnmf_score(X, k):
    symnmf_res = get_matrix_by_goal('symnmf', X, k)
    symnmf_labels = np.argmax(symnmf_res, axis=1)  # Pick the cluster with max weight for each point
    return silhouette_score(X, symnmf_labels)

def get_kmeans_scroe(X, k):
    centroids = get_kmeans_centroids(X, k, max_iter, eps)
    kmeans_labels = [get_argmin_k(dp, centroids) for dp in X]
    return silhouette_score(X, kmeans_labels)

def main():
    if len(sys.argv) != 3:
        print("An Error Has Occurred")
        return
    
    try:
        k = int(sys.argv[1])
        X = np.loadtxt(sys.argv[2], delimiter=",", ndmin=2).tolist()
        symnmf_score = get_symnmf_score(X, k)
        kmeans_score = get_kmeans_scroe(X, k)
        print(f"nmf: {symnmf_score:.4f}")
        print(f"kmeans: {kmeans_score:.4f}")
    except Exception:
        print("An Error Has Occurred")
        return


if __name__ == "__main__":
    main()