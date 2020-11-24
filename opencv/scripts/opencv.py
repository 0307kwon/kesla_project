#!/usr/bin/env python
import rospy
import cv2
import numpy as np
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage
import math

from std_msgs.msg import String

gleft_avg = "61.99082329198616"
gright_avg = "-55.00797980144134"
gleft_x = "49"
gright_x = "305"

class Gray():
    def __init__(self):
        self.selecting_sub_image = "raw" # you can choose image type "compressed", "raw"

#        if self.selecting_sub_image == "compressed":
#            self._sub = rospy.Subscriber('/usb_cam/image_raw/compressed', CompressedImage, #self.callback, queue_size=1)
#        else:
#            self._sub = rospy.Subscriber('/usb_cam/image_raw', Image, self.callback, queue_size=1)


	if self.selecting_sub_image == "compressed":
            self._sub = rospy.Subscriber('/main_camera/image_raw', CompressedImage, self.callback, queue_size=1)
        else:
            self._sub = rospy.Subscriber('/main_camera/image_raw', Image, self.callback, queue_size=1)

        self.bridge = CvBridge()



    def callback(self, image_msg):

        if self.selecting_sub_image == "compressed":
            #converting compressed image to opencv image
            np_arr = np.fromstring(image_msg.data, np.uint8)
            cv_image = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
        elif self.selecting_sub_image == "raw":
            cv_image = self.bridge.imgmsg_to_cv2(image_msg, "bgr8")

	"""
	dst=cv_image.copy()
	offset = 190
	cut_image = cv_image[offset:240, 0:320]

	img_hsv = cv2.cvtColor(cut_image, cv2.COLOR_BGR2HSV)
	lower_yellow = (23, 41, 133)
	upper_yellow = (40, 150, 255)
	img_mask = cv2.inRange(img_hsv, lower_yellow, upper_yellow)
	#img_result = cv2.bitwise_and(img_color, img_color, mask = img_mask)
	#cv_gray = cv2.cvtColor(img_result, cv2.COLOR_RGB2GRAY)
	ret,cv_thres = cv2.threshold(img_mask, 127, 255, cv2.THRESH_BINARY)
  	gauss_gray = cv2.GaussianBlur(cv_thres,(3,3),3.0)
	canny_gray = cv2.Canny(gauss_gray, 50 ,150)

	#lines = cv2.HoughLines(canny_gray, 0.8, np.pi / 180, 150, srn = 100, stn = 200, min_theta = 0, max_theta = np.pi)
	lines = cv2.HoughLinesP(canny_gray, rho = 1, theta = np.pi / 180, threshold = 23, minLineLength = 5, maxLineGap = 100)
	"""




	dst=cv_image.copy()
	offset = 190
	cut_image = cv_image[offset:240, 0:320]

	img_hsv = cv2.cvtColor(cut_image, cv2.COLOR_BGR2HSV)
	lower_yellow = (23, 41, 133)
	upper_yellow = (40, 150, 255)
	img_mask = cv2.inRange(img_hsv, lower_yellow, upper_yellow)

        #cv_gray = cv2.cvtColor(cut_image, cv2.COLOR_RGB2GRAY)
	ret,cv_thres = cv2.threshold(img_mask, 127, 255, cv2.THRESH_BINARY)
        gauss_gray = cv2.GaussianBlur(cv_thres,(3,3),3.0)
	canny_gray = cv2.Canny(gauss_gray, 50 ,150)


	#lines = cv2.HoughLines(canny_gray, 0.8, np.pi / 180, 150, srn = 100, stn = 200, min_theta = 0, max_theta = np.pi)
	lines = cv2.HoughLinesP(canny_gray, rho = 1, theta = np.pi / 180, threshold = 23, minLineLength = 5, maxLineGap = 100)



	#if lines is not None:
	#	for i in lines:
    	#	    rho, theta = i[0][0], i[0][1]
        #	    a, b = np.cos(theta), np.sin(theta)
    	#	    x0, y0 = a*rho, b*rho
	#
    	#	    scale = cv_image.shape[0] + cv_image.shape[1]
	#
    	#	    x1 = int(x0 + scale * -b)
    	#	    y1 = int(y0 + scale * a)
    	#	    x2 = int(x0 - scale * -b)
    	#	    y2 = int(y0 - scale * a)
	#
    	#	    cv2.line(dst, (x1, y1), (x2, y2), (0, 0, 255), 2)
    	#	    cv2.circle(dst, (x0, y0), 3, (255, 0, 0), 5, cv2.FILLED)

	left_sum=0
	right_sum=0
	avg=0
	left = np.array([])
	right = np.array([])
	#print("=================start======================")

	if lines is not None:
        	temp_lx = 0
        	temp_rx = 320
		for i in lines:
			cv2.line(dst, (i[0][0], i[0][1]+offset), (i[0][2], i[0][3]+offset), (0, 0, 255), 2)
			if i[0][2] < 160:

                		if temp_lx < max(i[0][2],i[0][0]):
                    			temp_lx = max(i[0][2],i[0][0])

				#print(left)
				gradient = -float((float(i[0][3])-float(i[0][1]))/(float(i[0][2])-float(i[0][0])))
				angular = math.degrees(math.atan(gradient))
				left = np.append(left,[angular])
				#print("gradient:","%0.2f"%gradient)
				#print("left_angular:",angular)
			else:
				if temp_rx > min(i[0][2],i[0][0]):
                    			temp_rx = min(i[0][2],i[0][0])
				#print(right)
				gradient = -float((float(i[0][3])-float(i[0][1]))/(float(i[0][2])-float(i[0][0])))
				angular = math.degrees(math.atan(gradient))
				right = np.append(right,[angular])
				#print("gradient:","%0.2f"%gradient)
				#print("right_angular:",angular)
		global gleft_x
		gleft_x = temp_lx
		global gright_x
		gright_x = temp_rx

		if len(left)==0:
			left_avg = "empty"
			right_length = len(right)
			for i in range(right_length):
				right_sum = right_sum + right[i]
				right_avg = right_sum / right_length

			#print("right", right_avg)
			#print("sum",right_avg)

		if len(right)==0:
			right_avg = "empty"
			left_length = len(left)
			for i in range(left_length):
				left_sum = left_sum + left[i]
				left_avg = left_sum / left_length

			#print("left", left_avg)
			#print("sum",left_avg)

		if  len(left) != 0 and len(right) != 0:
			left_length = min(len(left),len(right))
			right_length = min(len(left),len(right))
			#print("left,right",left_length,right_length)
			for i in range(left_length):
				left_sum = left_sum + left[i]
				right_sum = right_sum + right[i]
				left_avg = left_sum / left_length
				right_avg = right_sum / right_length

		#print("left, right", left_avg,right_avg)
		#print("sum",left_avg+right_avg)
	else:
		left_avg = "empty"
		right_avg = "empty"



	global gleft_avg, gright_avg
	gleft_avg = left_avg
	gright_avg = right_avg

	#print("left, right", left_avg,right_avg)
	#print("=================end======================")

	cv2.imshow('dst', dst),cv2.waitKey(1)




    def main(self):
	global gleft_avg,gleft_x,gright_avg,gright_x
	pub = rospy.Publisher('/kesla/gradient',String,queue_size=10)
	rate = rospy.Rate(20)#20hz
	while not rospy.is_shutdown():
		hello_str = "%s,%s,%s,%s"% (gleft_avg,gleft_x,gright_avg,gright_x)
	        pub.publish(hello_str)
       		rate.sleep()


	rospy.spin()




if __name__ == '__main__':
    rospy.init_node('gray',anonymous = True)
    node = Gray()
    node.main()
