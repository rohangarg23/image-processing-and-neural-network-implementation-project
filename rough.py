import numpy as np
import matplotlib.pyplot as plt
import cv2

img = cv2.imread('./data/dog.jpg',cv2.IMREAD_COLOR)
#IMREAD_COLOR = 1
#IMREAD_UNCHANGED = -1
#IMREAD_GRAYSCALE =0
# photo = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyallwindows()
# plt.imshow(img,cmap='gray',interpolation='bicubic')
# plt.plot([50,100],[80,100],'c',linewidth=5)
# plt.show()
# cv2.imwrite('watchgray.png',img) 