#coding:utf-8
from __future__ import division, print_function

import tensorflow as tf
import numpy as np

# import MNIST data
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('./tmp/data/', one_hot=True)

# Parameters
learning_rate = 0.001
batch_size = 100
display_step = 1
model_path = "./tmp/model.ckpt"

# Network Parameters
n_hidden_1 = 256 # 1st layer neural
n_hidden_2 = 256 # 2rd layer neural
n_input = 784
n_classes = 10

X = tf.placeholder(tf.float32, shape=[None, n_input])
Y = tf.placeholder(tf.float32, shape=[None, n_classes])

# Create model
def multiplayer_perceptron(x, weights, biases):
    # Hidden layer with RELU activation
    layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
    layer_1 = tf.nn.relu(layer_1)
    # Hidden layer with RELU activation
    layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
    layer_2 = tf.nn.relu(layer_1)

    out_layer = tf.matmul(layer_2, weights['out']) + biases['out']
    return out_layer

# Store layer weight & bias
weights = {
        'h1': tf.Variable(tf.random_normal([n_input, n_hidden_1])),
        'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'out' : tf.Variable(tf.random_normal([n_hidden_2, n_classes]))
}

biases = {
        'b1' : tf.Variable(tf.random_normal([n_hidden_1])),
        'b2' : tf.Variable(tf.random_normal([n_hidden_2])),
        'out' : tf.Variable(tf.random_normal([n_classes]))
}

# Construct model
pred = multiplayer_perceptron(X, weights, biases)

# loss and Optimizer

loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(loss)

# initialize
init = tf.global_variables_initializer()

# 'Saver' op to save and restore all the Variables
saver = tf.train.Saver()

print("start 1st running")
with tf.Session() as sess:
    # Run the initalizer
    sess.run(init)

    # Training cycle
    for epoch in range(3):
        avg_loss = 0.
        total_batch = int(mnist.train.num_examples/batch_size)
        # Loop over all batches
        for i in range(total_batch):
            batch_x, batch_y = mnist.train.next_batch(batch_size)
            # Run optimization Op
            _, c = sess.run([optimizer, loss], feed_dict={X: batch_x, Y: batch_y})

            # compute average loss
            avg_loss += c / total_batch
        print("Epoch:", '%04d' % (epoch + 1), 'loss=',\
              "{:.9f}".format(avg_loss))
    print("First Optimization Finished!")

    # Test model
    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(Y, 1))
    # Calculate accuracy
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
    print("Accracy:", accuracy.eval({X : mnist.test.images, Y: mnist.test.labels}))

    # Save model weights to disk
    save_path = saver.save(sess, model_path)
    print("Model saved in file: %s" % save_path)

# Running a new Session
print("Starting 2nd session...")
with tf.Session() as sess:
    # initialize Variable
    sess.run(init)

    # Restore model weighs from previously saved model
    saver.restore(sess, model_path)
    #print("Model restored from file: %s" % save_path)

    # Resume Training
    for epoch in range(7):
        avg_loss = 0.
        total_batch = int(mnist.train.num_examples / batch_size)
        # Loop over all batches
        for i in range(total_batch):
            batch_x, batch_y = mnist.train.next_batch(batch_size)
            # Run optimization op (backprop) and cost op (to get loss value)
            _, c = sess.run([optimizer, loss], feed_dict={X: batch_x,
                                                          Y: batch_y})
            # Compute average loss
            avg_loss += c / total_batch
        # Display logs per epoch step
        if epoch % display_step == 0:
            print("Epoch:", '%04d' % (epoch + 1), "cost=", \
                "{:.9f}".format(avg_loss))
    print("Second Optimization Finished!")

    # Test model
    correct_prediction = tf.equal(tf.argmax(pred, 1), tf.argmax(Y, 1))
    # Calculate accuracy
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
    print("Accuracy:", accuracy.eval(
        {X: mnist.test.images, Y: mnist.test.labels}))
