/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.spark.sql

import edu.berkeley.cs.rise.opaque.logical.OpaqueOperator
import edu.berkeley.cs.rise.opaque.logical.Encrypt
import edu.berkeley.cs.rise.opaque.logical.Clip2Norm
import edu.berkeley.cs.rise.opaque.logical.ClipInfNorm
import edu.berkeley.cs.rise.opaque.logical.LrGradient
//import edu.berkeley.cs.rise.opaque.logical.HuberSvmGradient
import edu.berkeley.cs.rise.opaque.logical.AddLaplaceNoise
import edu.berkeley.cs.rise.opaque.logical.AddGaussianNoise
//import org.apache.spark.sql.catalyst.encoders.RowEncoder

class OpaqueDatasetFunctions[T](ds: Dataset[T]) extends Serializable {

  def encrypted(): DataFrame = {
    Dataset.ofRows(ds.sparkSession, Encrypt(false, ds.logicalPlan))
  }

// clip samples according to different norms

  def clip_l2(bound: Double): DataFrame = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, Clip2Norm(bound, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }

  def clip_linf(bound: Double): DataFrame = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, ClipInfNorm(bound, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }




// gradient of different loss functions
  def lr_gradient(regterm: Double, theta: Seq[Double]): Seq[Double] = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator]){
      val df = Dataset.ofRows(ds.sparkSession, LrGradient(regterm, theta, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
      val dl = df.collect.map(_.toSeq.asInstanceOf[Seq[Double]].toList).toList
      val height = dl.length
      val width = dl(0).length
      var thetaArray = new Array[Double](width-1)
      for(i<-0 to width-2) {
        for(j<-0 to height-1) {
          thetaArray(i) += dl(j)(i)
        }
      }
      for(i<-0 to width-2) {
        thetaArray(i) /= height
      }
      return thetaArray.toList.toSeq.asInstanceOf[Seq[Double]]
    }
    else {
      println("Please encrypt the DataFrame first!")
      val df = Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
      return df.first.toSeq.asInstanceOf[Seq[Double]]
    }
  }



// different noise we need to implement differential private algorithms.
  def add_laplace_noise(noise_para: Double): DataFrame = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, AddLaplaceNoise(noise_para, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }

  def add_gaussian_noise(noise_para: Double): DataFrame = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, AddGaussianNoise(noise_para, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }



/*
// complete algorithms
  def logistic_regression(regterm: Double): DataFrame = {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, LogisticRegression(regterm, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }

// complete dp algorithms
  def dp_logistic_regression(regterm: Double, eps: Double, delta: Double): DataFrame {
    if(ds.logicalPlan.isInstanceOf[OpaqueOperator])
      return Dataset.ofRows(ds.sparkSession, DPLogisticRegression(regterm, eps, delta, ds.logicalPlan.asInstanceOf[OpaqueOperator]))
    else {
      println("Please encrypt the DataFrame first!")
      return Dataset.ofRows(ds.sparkSession, ds.logicalPlan)
    }
  }*/
}
