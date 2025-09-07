import math

def get_euclidean_distance(point1, point2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(point1, point2)))

def get_argmin_k(datapoint, centroids):
    argmin_k = 0
    min_distance = get_euclidean_distance(datapoint, centroids[0])
    for i in range(1, len(centroids)):
        distance = get_euclidean_distance(datapoint, centroids[i])
        if distance < min_distance:
            min_distance = distance
            argmin_k = i
    return argmin_k

def update_centroid(datapoints, d):
    if not datapoints:
        return [0] * d # Return zero vector if cluster is empty
    coordinate_sums = [0] * d
    for datapoint in datapoints:
        for i in range(d):
            coordinate_sums[i] += datapoint[i]
    return [x / len(datapoints) for x in coordinate_sums]

def get_kmeans_centroids(datapoints, K, max_iter=400, epsilon=0.001):
    d = len(datapoints[0])
    centroids = datapoints[:K] # Initialize centroids as first K points
    delta_centroids = [False] * K
    iteration = 0

    # Loop until centroids stabilize or max iterations reached
    while any(not x for x in delta_centroids) and iteration < max_iter:
        clusters = [[] for _ in range(K)]

        # Assign each point to the closest centroid
        for dp in datapoints:
            argmin_k = get_argmin_k(dp, centroids)
            clusters[argmin_k].append(dp)

        # Update centroids based on assigned points
        for i in range(K):
            old_centroid = centroids[i]
            new_centroid = update_centroid(clusters[i], d)
            centroids[i] = new_centroid

            # Check if centroid moved less than epsilon (converged)
            if get_euclidean_distance(old_centroid, new_centroid) < epsilon:
                delta_centroids[i] = True
            else:
                delta_centroids[i] = False

        iteration += 1

    return centroids