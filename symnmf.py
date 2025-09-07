import sys
import numpy as np

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

    print(f"k = {k}")
    print(f"goal = {goal}")
    print(f"path = {path}")

    try:
        # Try to load the CSV file using NumPy
        data = np.loadtxt(path, delimiter=",")
        print(data)
    except Exception:
        print("An Error Has Occurred")

if __name__ == "__main__":
    main()