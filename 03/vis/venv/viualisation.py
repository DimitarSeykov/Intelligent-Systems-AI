import os
import numpy as np
import cv2

def read() -> np.ndarray:
    data = np.genfromtxt(r'../../Travelling Salesman/Travelling Salesman/output/output_vis.txt', delimiter=',')
    shape = data.shape
    data = data[~np.isnan(data)]
    data = data.reshape((shape[0], 100, 2))

    return data


def draw_dots(data: np.ndarray) -> np.ndarray:
    image = np.zeros((900, 900, 3))
    for t in data[0]:
        image[int(t[1]), int(t[0])] = [255, 255, 255]
    return image

def draw(data: np.ndarray):

    image = draw_dots(data)

    window_name = 'image'
    cv2.imshow(window_name, image)
    cv2.waitKey(0)

    for row in data[1:]:
        lastTuple = row[0]
        lastTuple = tuple([int(lastTuple[0]), int(lastTuple[1])])
        # lines = 0
        for (t, p) in row[1:]:
            cv2.line(image, tuple(lastTuple), tuple([int(t), int(p)]), (255,255,255), 1)
            # lines += 1
            lastTuple = tuple([int(t), int(p)])
        cv2.imshow(window_name, image)
        cv2.waitKey(0)
        # print(lines)
        image = draw_dots(data)



if __name__ == '__main__':
    data = read()
    draw(data)

