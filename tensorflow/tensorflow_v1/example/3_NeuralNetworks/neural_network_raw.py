#coding:utf-8
""" Neural Nerwork
    全连接层是最原始，出现最早的神经网络结构，能够无损的传递特征信息，但是网络参数多，计算量大，训练较慢
"""
from __future__ import division, print_function

import tensorflow as tf
import numpy as np

# import MNIST data
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('./tmp/data/', one_hot=True)

import tensorflow as tf

# Parameters
learning_rate = 0.01
num_steps = 500
batch_size = 128
display_steps = 100

# Network Parameters
n_hidden_1 = 128
n_hidden_2 = 128
num_input = 784
num_classes = 10

# tf Graph input
X = tf.placeholder(tf.float32, shape=[None, num_input])
Y = tf.placeholder(tf.float32, shape=[None, num_classes])

# Store layers weght & bias
weights = {
        'hidden_1': tf.Variable(tf.random_normal([num_input, n_hidden_1])),
        'hidden_2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'out' : tf.Variable(tf.random_normal([n_hidden_2, num_classes]))
}

biases = {
        'b1' : tf.Variable(tf.random_normal([n_hidden_1])),
        'b2' : tf.Variable(tf.random_normal([n_hidden_2])),
        'out': tf.Variable(tf.random_normal([num_classes]))
}

# Create Model
def neural_net(x):
    # Hidden fully connected layer with 128 neurals
    # Hidden fully connected layer with 128 neurals
    # output fully connected layer with a neural for each class

    layer1 = tf.layers.dense(x, n_hidden_1) #weights['hidden_1'], biases['b1'])
    layer2 = tf.layers.dense(layer1, n_hidden_2) #weights['hidden_2'], biases['b2'])
    out_layer = tf.layers.dense(layer2, num_classes) #weights['out'], biases['out'])
    ''' 等价
    layer_1 = tf.add(tf.matmul(x, weights['hidden_1']), biases['b1'])
    layer_2 = tf.add(tf.matmul(layer_1, weights['hidden_2']), biases['b2'])
    out_layer = tf.add(tf.matmul(layer_2, weights['out']), biases['out'])
    '''
    return out_layer


# Construct Model
logits = neural_net(X)
prediction = tf.nn.softmax(logits)

# Define loss and optimizer
loss_op = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=Y))

optimizer = tf.train.AdamAsyncOptimizer(learning_rate=learning_rate)
train_op = optimizer.minimize(loss_op)

# Evaluate model
correct_pred = tf.equal(tf.argmax(prediction, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

# initialize
init = tf.global_variables_initializer()

# Start training
with tf.Session() as sess:
    # Run the initializer
    sess.run(init)

    for step in range(1, num_steps + 1):
        batch_x, batch_y = mnist.train.next_batch(batch_size)

        # Run optimizer op (backprop)
        sess.run(train_op, feed_dict={X: batch_x, Y: batch_y})
        if step % display_steps == 0 or step == 1:
            # Calculate batch Loss and accuracy
            loss, acc = sess.run([loss_op, accuracy], feed_dict={X: batch_x, Y: batch_y})

            print("Step " + str(step) + ", Minibatch Loss= " + \
                  "{:.4f}".format(loss) + ", Training accuracy= " + \
                  "{:.3f}".format(acc))

    print("Optimization Finished!")

    # Calculate accuracy for MNIST test images
    print("Testing Accuracy:", \
          sess.run(accuracy, feed_dict={X: mnist.test.images, Y:mnist.test.labels}))
