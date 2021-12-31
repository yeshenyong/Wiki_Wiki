from __future__ import print_function

import tensorflow as tf


# Basic constant operation
# The value returned by the construtor represents the output
# of the constant op
a = tf.constant(2)
b = tf.constant(3)

# Lanuch the default graph
with tf.Session() as sess:
    print("a = 2, b = 3")
    print("Addition with constant: %i" % sess.run(a + b))
    print("Multiplication with constant: %i" % sess.run(a * b))

# Basic operation with Variable as graph input
# The value returned by the construtor represents the output
a = tf.placeholder(dtype=tf.int16)
b = tf.placeholder(dtype=tf.int16)

# Define some operation
add = tf.add(a, b)
mul = tf.multiply(a, b)

# Launch the default graph.
with tf.Session() as sess:
    # Run every operation with Variable input
    print("Addition with variables: %i" % sess.run(add, feed_dict={a:2, b:3}))
    print("Multiplication with variables: %i" % sess.run(mul, feed_dict={a:2, b:3}))

matrix1 = tf.constant([[3., 3.]])
matrix2 = tf.constant([[2.], [2.]])

product = tf.matmul(matrix1, matrix2)
with tf.Session() as sess:
    result = sess.run(product)
    print(result)
