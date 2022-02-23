#include "scrfd_crop.hpp"

//  AlignFace Funtion is Created by
//  Created by vealocia on 2019/10/17.
//  Copyright © 2019 vealocia. All rights reserved.
static int AlignFace(cv::Mat &img_src, const std::vector<FaceObject> &faceobjects, std::vector<cv::Mat>& aligned_faces)
{
    float points_dst[5][2] = {
        {30.2946f + 8.0f, 51.6963f},
        {65.5318f + 8.0f, 51.5014f},
        {48.0252f + 8.0f, 71.7366f},
        {33.5493f + 8.0f, 92.3655f},
        {62.7299f + 8.0f, 92.2041f}};

    std::cout << "start align face." << std::endl;
    if (img_src.empty())
    {
        std::cout << "input empty." << std::endl;
        return 10001;
    }

    for (int i = 0; i < faceobjects.size(); i++)
    {
        const FaceObject &obj = faceobjects[i];
        float points_src[5][2] = {
            {obj.landmark[0].x, obj.landmark[0].y},
            {obj.landmark[1].x, obj.landmark[1].y},
            {obj.landmark[2].x, obj.landmark[2].y},
            {obj.landmark[3].x, obj.landmark[3].y},
            {obj.landmark[4].x, obj.landmark[4].y}
        };

        cv::Mat src_mat(5, 2, CV_32FC1, points_src);
        cv::Mat dst_mat(5, 2, CV_32FC1, points_dst);

        cv::Mat transform = FacePreprocess::similarTransform(src_mat, dst_mat);

        //face_aligned->create(112, 112, CV_32FC3);

        cv::Mat cropped_temp;

        cv::Mat transfer_mat = transform(cv::Rect(0, 0, 3, 2));
        cv::warpAffine(img_src.clone(), cropped_temp, transfer_mat, cv::Size(112, 112), 1, 0, 0);

        aligned_faces.push_back(cropped_temp);        
        cropped_temp.release();
    }   
    std::cout << "end align face. : " << faceobjects.size() << std::endl; 
    return 0;
}


static SCRFD c_scrfd("scrfd_500m_kps-opt2.bin", "scrfd_500m_kps-opt2.param");

std::vector<cv::Mat> scrfd::crop_faces(cv::Mat frame)
{
  std::vector<FaceObject> face_objects;
  c_scrfd.detect(frame, face_objects);

  std::vector<cv::Mat> cropped_faces;
  AlignFace(frame, face_objects, cropped_faces);

  return cropped_faces;
}
