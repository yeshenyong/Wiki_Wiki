from __future__ import absolute_import, division, print_function

import tensorflow as tf
import numpy as np
import tensorflow.contrib.eager as tfe

# Set Eager API
print("Setting Eager mode...")
tfe.enable_eager_execution()

# Define constant tensors
print("Define constant tensors")
a = tf.constant(2.)
print("a = %i" % a)
b = tf.constant(3.)
print("b = %i" % b)


# Run the operation without the need for tf.Session
print("Running operations, without tf.Session")
c = a + b
print("a + b = %i" % c)
d = a * b
print("a * b = %i" % d)

# Full compatibility with Numpy
print("Mixing operations with Tensors and Numpy Arrays")

# Define constans tensors
a = tf.constant([[2., 1.],
                 [1., 0.]], dtype=tf.float32)
print("Tensor:\n a = %s" % a)

b = np.array([[3., 0.],
              [1., 0.]], dtype=np.float32)

print("NumpyArray:\n b=%s" % b)

# Run the operation without the need for tf.Session
print("Running operation without the need for tf.Session")
c = a + b
print("a + b = %s" % c)
d = tf.matmul(a, b)
print("a * b = %s" % d)

print("Iterate through Tensor 'a':")
for i in range(a.shape[0]):
    for j in range(a.shape[1]):
        print(a[i][j])
