from __future__ import print_function
import tensorflow as tf
import numpy

rng = numpy.random

# Parameters
learning_rate = 0.01
training_epochs = 1000
display_step = 50

# Traning Data
train_X = numpy.asarray([3.3,4.4,5.5,6.71,6.93,4.168,9.779,6.182,7.59,2.167,
                          7.042,10.791,5.313,7.997,5.654,9.27,3.1])
train_Y = numpy.asarray([1.7,2.76,2.09,3.19,1.694,1.573,3.366,2.596,2.53,1.221,
                         2.827,3.465,1.65,2.904,2.42,2.94,1.3])

n_samples = train_X.shape[0]

# tf Graph Input
X = tf.placeholder(dtype=tf.float32)
Y = tf.placeholder(dtype=tf.float32)

# Set model weights
W = tf.Variable(rng.randn(), name="weights")
b = tf.Variable(rng.randn(), name="bias")

# Construct a linear model
pred = tf.add(tf.multiply(X, W), b)

# Mean squared error
loss = tf.reduce_sum(tf.pow(pred - Y, 2)) / (2 * n_samples)

# Gradient descent
optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss)

# Initialize the Variables
init = tf.global_variables_initializer()

# Start training
with tf.Session() as sess:
    # Run the Initialize
    sess.run(init)

    # Fit all training Data
    for epoch in range(training_epochs):
        for (x, y) in zip(train_X, train_Y):
            sess.run(optimizer, feed_dict={X:x, Y:y})
        # Display logs per epoch steps
        if (epoch + 1) % display_step == 0:
            c = sess.run(loss, feed_dict={X:train_X, Y:train_Y})
            print("Epoch:", '%04d' % (epoch + 1), "cost=", "{:.9f}".format(c), \
                  "W=", sess.run(W), "b=", sess.run(b))
    print("optimizer finished!")
    training_loss = sess.run(loss, feed_dict={X:train_X, Y:train_Y})
    print("training loss=", training_loss, "W=", sess.run(W), sess.run(b))

    # Tesing example, as requested (Issue #2)
    test_X = numpy.asarray([6.83, 4.668, 8.9, 7.91, 5.7, 8.7, 3.1, 2.1])
    test_Y = numpy.asarray([1.84, 2.273, 3.2, 2.831, 2.92, 3.24, 1.35, 1.03])

    print("Testing.. (Mean square loss Comparsioin)")
    testing_loss = sess.run(tf.reduce_mean(tf.pow(pred - Y, 2)) / (2 * test_X.shape[0]), feed_dict={X: test_X, Y: test_Y}) # same function as loss above
    #for i in test_X:
    print("training_Y=", sess.run(pred, feed_dict={X: test_Y}))
    print("Tesing loss=", testing_loss)
    print("Absolute mean square loss difference:", abs(training_loss - testing_loss))
