/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, Andreas ten Pas
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LOCAL_FRAME_H
#define LOCAL_FRAME_H

#include <Eigen/Dense>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <vector>


typedef pcl::PointCloud<pcl::PointXYZRGBA> PointCloudRGBA;


/** LocalFrame class
 *
 * \brief Local reference frame estimation
 * 
 * This class estimates the local reference frame for a point neighborhood. The coordinate axes of the local frame are
 * the curvature axis, normal, and binormal.
 * 
*/
class LocalFrame
{
public:

  /**
   * \brief Constructor.
   * \param T_cams the camera poses
   * \param sample the sample for which the point neighborhood was found
  */
  LocalFrame(const std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d> >& T_cams,
    const Eigen::Vector3d& sample, int majority_cam_source);

  /**
   * \brief Estimate the average normal axis for the point neighborhood.
   * \param normals the 3xn matrix of normals found for points in the point neighborhood
  */
  void findAverageNormalAxis(const Eigen::MatrixXd& normals);

	/**
	 * \brief Set the input point cloud.
	 * \param input the input point cloud
	*/
	void setInputCloud(const PointCloudRGBA::Ptr& input)
	{
		this->input_ = input;
	}
	
	/**
	 * \brief Print a description of the local reference frame.
	*/
	void print();
	
	/**
	 * \brief Plot the local axes.
	 * \param viewer_void a pointer to a PCL visualizer
	 * \param id an identifier number for the axes
	*/
	void plotAxes(void* viewer_void, int id) const;
	
	/**
	 * \brief Return the sample for which the point neighborhood was found.
	 * \return the 3x1 sample for which the point neighborhood was found
	*/
	const Eigen::Vector3d& getSample() const
	{
		return sample_;
	}
	
	/**
	 * \brief Return the binormal for the quadric fitted to the point neighborhood.
	 * \return the 3x1 binormal for the quadric fitted to the point neighborhood
	*/
	const Eigen::Vector3d& getBinormal() const
	{
		return binormal_;
	}
	
	/**
	 * \brief Return the curvature axis for the quadric fitted to the point neighborhood.
	 * \return the 3x1 curvature axis for the quadric fitted to the point neighborhood
	*/
	const Eigen::Vector3d& getCurvatureAxis() const
	{
		return curvature_axis_;
	}
	
	/**
	 * \brief Return the normal for the quadric fitted to the point neighborhood.
	 * \return the 3x1 normal for the quadric fitted to the point neighborhood
	*/
	const Eigen::Vector3d& getNormal() const
	{
		return normal_;
	}

	/**
	 * \brief Set the sample for the point neighborhood.
	 * \param sample the sample to be used
	 */
  void setSample(const Eigen::Vector3d& sample)
  {
    sample_ = sample;
  }


private:

  Eigen::Matrix3Xd cam_origins_; ///< the camera positions
  Eigen::Vector3d sample_; ///< the sample for which the point neighborhood was found
  int majority_cam_source_; ///< the majority camera source
  PointCloudRGBA::Ptr input_; ///< the input point cloud
  Eigen::Vector3d curvature_axis_, normal_, binormal_; ///< the curvature, normal, and binormal axis
  double normals_ratio_; ///< the ratio between the normals of the quadric
};

#endif // PCL_FEATURES_CURVATURE_ESTIMATION_TAUBIN_H_
