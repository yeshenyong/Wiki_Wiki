""" cpu and gpu calcluate diff
    and GPU using methods
"""
from __future__ import division, print_function

import tensorflow as tf
import numpy as np

with tf.device('/gpu:0'):
    a = tf.constant([1., 2., 3.], shape=[3], name='a')
    b = tf.constant([2., 3., 4.], shape=[3], name='b')

with tf.device('/gpu:0'):
    #c = tf.add(a, b)
    c = a + b

# initializer
init = tf.global_variables_initializer()

sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))
sess.run(init)
sess.run(c)
#
#  在默认情况下，即使机器有多个cpu，Tensorflow也不会去区分它们，统一使用/cpu:0。
#  而同一台机器上不同GPU的名称是不同的，如/gpu:0，/gpu:1等。
#  默认情况下，Tensorflow优先使用GPU。
#  需要注意的是，在Tensorflow上，不是所有的操作都可以放在GPU上的，如
#  为了避免这个问题，可以在生成Session时指定allow_soft_placement=True，当运算无法在GPU上执行时，会自动将运算放到CPU上。
#  用法：
#  sess = tf.Session(config=tf.ConfigProto(log_device_placement=True,allow_soft_placement=True))
#  Tensorflow会默认占用设备所有GPU以及每个GPU上的显存，如果只使用部分GPU可以：
#  （注：虽然占用所有GPU，但是会优先使用/GPU:0）
#  在代码中使用
# import os

# os.environ['CUDA_VISIBLE_DEVICES'] = '0,1'

#  TensorFlow默认一次性占用GPU的所有显存，但是也支持动态分配GPU的显存，使得不会一开始就占满所有显存。
#  config = tf.ConfigProto()
#  config.gpu_options.allow_growth = True
#  也可以直接按固定的比例分配
#  config.gpu_options.per_process_gpu_memory_fraction = 0.4
#  sess = tf.Session(config=config)
#  log_device_placement：将运行每一个操作的设备输出到屏幕上。
#  allow_soft_placement：将GPU上不能运行的运算自动放到CPU上运行。
#  allow_growth：动态分配GPU显存。
#  per_process_gpu_memory_fraction：按比例分配GPU显存。
#
