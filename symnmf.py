import sys
import numpy as np
import symnmfmodule

def get_initial_H(W, k):
    np.random.seed(1234)
    m = np.mean(W)
    return np.random.uniform(0, 2 * np.sqrt(m / k), size=(len(W), k))

# symnmfmodule.symnmf(H, W, len(X), k, 300, 1e-4)

def get_matrix_by_goal(goal, X, k):
    if goal == 'sym':
        return symnmfmodule.sym(X)
    elif goal == 'ddg':
        return symnmfmodule.ddg(X)
    elif goal == 'norm':
        return symnmfmodule.norm(X)
    elif goal == 'symnmf':
        W = symnmfmodule.norm(X)
        for vector in W:
            print(','.join(f'{element:.4f}' for element in vector))
        H = get_initial_H(W, k)
        for vector in H:
            print(','.join(f'{element:.4f}' for element in vector))
        return symnmfmodule.symnmf(H, W, len(X), k, 300, 1e-4)
    return X

def main():
    if len(sys.argv) != 4:
        print("An Error Has Occurred")
        return

    k_arg = sys.argv[1]
    goal = sys.argv[2]
    path = sys.argv[3]

    try:
        k = int(k_arg)
    except ValueError:
        print("An Error Has Occurred")
        return

    try:
        X = np.loadtxt(path, delimiter=",")
    except Exception:
        print("An Error Has Occurred")
        return
    
    try:
        res = get_matrix_by_goal(goal, X.tolist(), k)
    except SystemError:
        print("An Error Has Occurred")
        return

    for vector in res:
        print(','.join(f'{element:.4f}' for element in vector))
    

if __name__ == "__main__":
    main()