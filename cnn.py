


from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
import tensorflow as tf
import linked as dl
import agr
tf.logging.set_verbosity(tf.logging.INFO)


def cnn_model_fn(features, labels, mode):
  """Model function for CNN."""
  if labels == None:
    #do nothing
    randomaomaoma=1
  else:
    print(tf.Print(labels,[labels]))
    input_layer = tf.reshape(features['x'], [-1, 250, 250, 6])

  
  conv1 = tf.layers.conv2d(
      inputs=input_layer,
      filters=36,
      kernel_size=[5, 5],
      padding="same",
      activation=tf.nn.relu)

  
  pool1 = tf.layers.max_pooling2d(inputs=conv1, pool_size=[2, 2], strides=2)

  
  conv2 = tf.layers.conv2d(
      inputs=pool1,
      filters=72,
      kernel_size=[5, 5],
      padding="same",
      activation=tf.nn.relu)

  
  pool2 = tf.layers.max_pooling2d(inputs=conv2, pool_size=[3, 3], strides=2)
  conv3 = tf.layers.conv2d(
      inputs=pool2,
      filters=114,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)
  pool3 = tf.layers.max_pooling2d(inputs=conv3, pool_size=[3, 3], strides=2)
  
  conv4 = tf.layers.conv2d(
      inputs=pool3,
      filters=114,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)
  pool4 = tf.layers.max_pooling2d(inputs=conv4, pool_size=[2, 2], strides=2)
  conv5 = tf.layers.conv2d(
      inputs=pool4,
      filters=228,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)
  pool5 = tf.layers.max_pooling2d(inputs=conv5, pool_size=[3, 3], strides=2)
  
  conv6 = tf.layers.conv2d(
      inputs=pool5,
      filters=456,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)
  pool6 = tf.layers.max_pooling2d(inputs=conv6, pool_size=[2, 2], strides=3)
  
  conv7 = tf.layers.conv2d(
      inputs=pool6,
      filters=912,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)
  pool7 = tf.layers.max_pooling2d(inputs=conv7,pool_size=[2,2],strides=1)
  
 
  pool2_flat = tf.reshape(pool7, [-1, 1 * 1 * 912])

  
  dense = tf.layers.dense(inputs=pool2_flat, units=912, activation=tf.nn.relu)

  
  dropout = tf.layers.dropout(
      inputs=dense, rate=0.4, training=mode == tf.estimator.ModeKeys.TRAIN)
  dense2=tf.layers.dense(inputs=dropout, units=30, activation=tf.nn.relu)
  
  logits = tf.layers.dense(inputs=dense2, units=2)

  predictions = {
      
      "classes": tf.argmax(input=logits, axis=1),
      .
      "probabilities": tf.nn.softmax(logits, name="softmax_tensor")
  }
  if mode == tf.estimator.ModeKeys.PREDICT:
    return tf.estimator.EstimatorSpec(mode=mode, predictions=predictions)
  
  
  loss = tf.losses.sparse_softmax_cross_entropy(labels=labels, logits=logits)

  
  if mode == tf.estimator.ModeKeys.TRAIN:
    optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.001)
    train_op = optimizer.minimize(
        loss=loss,
        global_step=tf.train.get_global_step())
    return tf.estimator.EstimatorSpec(mode=mode, loss=loss, train_op=train_op)

  
  eval_metric_ops = {
      "accuracy": tf.metrics.accuracy(
          labels=labels, predictions=predictions["classes"])}
  return tf.estimator.EstimatorSpec(
      mode=mode, loss=loss, eval_metric_ops=eval_metric_ops)


def main(unused_argv):
  ctrl_var = open('control.txt','r').read()
  print(ctrl_var)
  if ctrl_var=='y':
    
  
    
    td,tl,ed,el = dl.loadin()

    train_data = np.asarray(td,dtype=np.float16) # Returns np.array
    train_labels = np.asarray(tl,dtype=np.int32)
    print(train_labels.shape)
    eval_data = np.asarray(ed,dtype=np.float16)  # Returns np.array
    eval_labels = np.asarray(el,dtype=np.int32)

  
  mnist_classifier = tf.estimator.Estimator(
      model_fn=cnn_model_fn, model_dir="C:/Users/Sournav/Desktop/tamu_491/New folder (2)/sqr/3dod_test1")
  
  
  tensors_to_log = {"probabilities": "softmax_tensor"}
  logging_hook = tf.train.LoggingTensorHook(
      tensors=tensors_to_log, every_n_iter=10)
  if ctrl_var=='y':
  # Train the model
    train_input_fn = tf.estimator.inputs.numpy_input_fn(
        x={"x": train_data},
        y=train_labels,
        batch_size=10,
        num_epochs=None,
        shuffle=True)
    mnist_classifier.train(
        input_fn=train_input_fn,
        steps=2000,
        hooks=[logging_hook])

    # Evaluate the model and print results
    eval_input_fn = tf.estimator.inputs.numpy_input_fn(
        x={"x": eval_data},
        y=eval_labels,
        num_epochs=1,
        shuffle=False)
    eval_results = mnist_classifier.evaluate(input_fn=eval_input_fn)
    print(eval_results)
  else:
    val=100
    for i in range(89,val):
      if i in [74,75]:
        continue
      data=np.asarray(dl.loadin_pred('../scrnsht/screen'+str(i)+'.pcd'),dtype=np.float16)
      #print(data)
      #blank=np.asarray([1],np.int32)
      predict_input_fn = tf.estimator.inputs.numpy_input_fn(
        x={"x":data},
        batch_size=1,
        num_epochs=1,
        shuffle=False)
      
      predictions = mnist_classifier.predict(input_fn=predict_input_fn)
      preds=next(predictions)
      print(preds['classes'])
      agr.inp(i,str(preds['classes']))
      
      
    


if __name__ == "__main__":
  tf.app.run()
