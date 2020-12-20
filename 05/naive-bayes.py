import numpy as np

ROWS = 0
COLS = 0
FOLD_NUM = 10
PERCENT_DEM = 0
PERCENT_REP = 0


def read_data() -> np.ndarray:
    file_data = np.genfromtxt("house-votes-84.data", dtype="str", delimiter=',')
    np.random.shuffle(file_data)
    global ROWS, COLS
    ROWS = file_data.shape[0]
    COLS = file_data.shape[1]
    return file_data


def make_splits() -> list:
    splits = []
    fold = ROWS / FOLD_NUM
    current = 0
    for i in range(FOLD_NUM):
        second = int(current + fold)
        if i == FOLD_NUM - 1:
            second = ROWS - 1
        splits.append((current, second))
        current = int(current + fold)

    return splits


def get_basic_percents(data: np.ndarray, fold: tuple) -> tuple:
    count_dem = 0;
    count_rep = 0;
    total = ROWS
    for i in range(ROWS):
        if fold[0] <= i <= fold[1]:
            total = total - 1
            continue
        if data[i][0] == "democrat":
            count_dem += 1
        else:
            count_rep += 1

    return count_dem / total, count_rep / total


def get_probability(data: np.ndarray, fold: tuple, basic_percentages: tuple) -> float:
    correct_guesses = 0
    for rng in range(fold[0], fold[1] + 1):
        vote = data[rng]  # vote looks something like: [democrat,y,n,y,y,y,n,n,y,?,y,y,n,n,y,n,y]
        prod_dem = basic_percentages[0]
        prod_rep = basic_percentages[1]
        for i in range(COLS):
            if fold == 0:
                continue
            count_dem = 1
            count_rep = 1
            for j in range(ROWS):
                if fold[0] <= j <= fold[1]:
                    continue
                if data[j][i] == vote[i]:
                    if data[j][0] == "democrat":
                        count_dem += 1
                    if data[j][0] == "republican":
                        count_rep += 1
            sum = count_dem + count_rep
            prod_dem *= (count_dem / sum)
            prod_rep *= (count_rep / sum)
            result = "unknown"

        result = "unknown"
        if prod_dem > prod_rep:
            result = "democrat"
        else:
            result = "republican"

        if result == vote[0]:
            correct_guesses += 1
        # print(prod_dem, " ", prod_rep)

    total = fold[1] - fold[0] + 1
    accuracy = correct_guesses / total
    return accuracy


if __name__ == '__main__':
    data = read_data()
    folds = make_splits()
    total = 0
    for i in range(FOLD_NUM):
        basic_percentages = get_basic_percents(data, folds[i])
        current_accuracy = get_probability(data, folds[i], basic_percentages)
        print("Accuracy for fold {}: {}".format(i + 1, current_accuracy))
        total += current_accuracy

    average_accuracy = total / FOLD_NUM
    print("Average accuracy is: ", average_accuracy)
