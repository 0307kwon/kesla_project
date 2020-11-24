#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from darknet_ros_msgs.msg import BoundingBoxes

detected = ""

class keslaYolo:
    def __init__(self):
        rospy.Subscriber("/darknet_ros/bounding_boxes", BoundingBoxes, self.callback)

    def callback(self, msg):
        global detected
        for i in range(len(msg.bounding_boxes)):
            if msg.bounding_boxes[0].Class == "person":
                rospy.loginfo(rospy.get_caller_id() + "I saw %s", msg.bounding_boxes[0].Class)
                detected = "true"
                rospy.loginfo(detected)
            else:
                rospy.loginfo(rospy.get_caller_id() + "I saw %s", msg.bounding_boxes[0].Class)
                detected = "false"

	    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", msg.bounding_boxes[0].Class)
	    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", len(msg.bounding_boxes))

    def main(self):
        global detected
	pub = rospy.Publisher('/kesla/yolo/personDetect',String,queue_size=10)
	rate = rospy.Rate(20)#20hz
	while not rospy.is_shutdown():
            if detected == "true":
	           pub.publish(detected)
            rate.sleep()
        rospy.spin()


if __name__ == '__main__':
    rospy.init_node('keslaYolo',anonymous = True)
    node = keslaYolo()
    node.main()
