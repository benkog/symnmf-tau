import sys
import numpy as np
import symnmfmodule

max_iter = 300
eps = 1e-4

def get_initial_H(W, k):
    np.random.seed(1234)
    m = np.mean(W)
    return np.random.uniform(0, 2 * np.sqrt(m / k), size=(len(W), k)).tolist()

def get_matrix_by_goal(goal, X, k):
    if goal == 'sym':
        return symnmfmodule.sym(X)
    elif goal == 'ddg':
        return symnmfmodule.ddg(X)
    elif goal == 'norm':
        return symnmfmodule.norm(X)
    elif goal == 'symnmf':
        W = symnmfmodule.norm(X)
        H = get_initial_H(W, k)
        return symnmfmodule.symnmf(H, W, len(X), k, max_iter, eps) 
    raise Exception # If the goal is invalid throw an exeption

def main():
    if len(sys.argv) != 4:
        print("An Error Has Occurred")
        return

    try:
        k = int(sys.argv[1])
        goal = sys.argv[2]
        X = np.loadtxt(sys.argv[3], delimiter=",").tolist()
        res = get_matrix_by_goal(goal, X, k)
        for row in res:
            print(','.join(f'{x:.4f}' for x in row))
    except Exception:
        print("An Error Has Occurred")    

if __name__ == "__main__":
    main()