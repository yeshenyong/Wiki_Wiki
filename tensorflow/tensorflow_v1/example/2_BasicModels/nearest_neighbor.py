from __future__ import print_function

import tensorflow as tf
import numpy as np

# Import MNIST data
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("./tmp/data/", one_hot=True)

# In this example, we limit mnist data
Xtr, Ytr = mnist.train.next_batch(5000) # 5000 for training (nn candidates)
Xte, Yte = mnist.test.next_batch(200) # 200 for testing

# tf Graph input
xtr = tf.placeholder("float", [None, 784])
xte = tf.placeholder("float", [784])


# Nearest Neighbor calculation using L1 Distance
# Calculate L1 Distance
distance = tf.reduce_sum(tf.abs(tf.add(xtr, tf.negative(xte))), reduction_indices=1)

# Prediction: Get min distance index (Nearest neighbor)
pred = tf.argmin(distance, 0)

accuracy = 0.

# Initializer the variables
init = tf.global_variables_initializer()

# Start training
with tf.Session() as sess:

    # Run the Initializer
    sess.run(init)

    # loop over test data
    for i in range(len(Xte)):
        # Get Nearest neighbor
        nn_index = sess.run(pred, feed_dict = {xtr: Xtr, xte: Xte[i, :]})

        # Get Nearest Neighbor class label and compare it to its true label
        print("Test", i, " Prediction:", np.argmax(Ytr[nn_index]), \
              "True class:", np.argmax(Yte[i]))

        # Calculate accuracy
        if np.argmax(Ytr[nn_index]) == np.argmax(Yte[i]):
            accuracy += 1./len(Xte)
    print("Xte type:", type(Xte))
    #print(Xte[0, :])
    print("Done!")
    print("Accuracy:", accuracy)
