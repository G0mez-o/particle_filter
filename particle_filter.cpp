#include <particle_filter.hpp>

int main(int argh, char* argv[])
{

  cv::VideoCapture cap(0);//デバイスのオープン
  //cap.open(0);//こっちでも良い．

  if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
  {
      //読み込みに失敗したときの処理
      return -1;
  }
  std::vector<particle> particles;
  for (int i = 0; i < 100; i++)
  {
    particle buffer_point;
    buffer_point.x = std::rand() % 640;
    buffer_point.y = std::rand() % 480;
    particles.push_back(buffer_point);
  }

  cv::Mat frame; //取得したフレーム
  while(cap.read(frame))//無限ループ
  {
      //
      //取得したフレーム画像に対して，クレースケール変換や2値化などの処理を書き込む．
      //
    // std::cout << frame.rows << std::endl;
    // std::cout << frame.cols << std::endl;
    // std::cout << frame.channels() << std::endl;
    // std::cout << frame.type() << std::endl;
    // cv::Vec3b buffer = frame.at<cv::Vec3b>(320, 320);
    // std::cout << buffer << std::endl;
    // std::cout << (int)buffer[0] << std::endl;
    // std::cout << (int)buffer[1] << std::endl;
    // std::cout << (int)buffer[2] << std::endl;
    // int center_x = particles[std::rand() % 100].x;
    // int center_y = particles[std::rand() % 100].y;
    int center_x;
    int center_y;
    double likehood_sum = 0.0;
    double buffer_parameter = 0.0;
    double max = 0.0;
    for (int i = 0; i < 100; i++)
    {
      cv::Vec3b color_buffer = frame.at<cv::Vec3b>(particles[i].y, particles[i].x);
      float color_dist = std::sqrt((255 -  color_buffer[0]) * (255 - color_buffer[0]) + (color_buffer[1]) * (color_buffer[1])
                                   + (255 - color_buffer[2]) * (255 - color_buffer[2]));
      particles[i].likehood = std::exp(- color_dist * color_dist / 10000);
    }
    for (int i = 0; i < 100; i++)
    {
      likehood_sum += particles[i].likehood;
    }
    for (int i = 0; i < 100; i++)
    {
      particles[i].likehood /= likehood_sum;
    }
    for (int i = 0; i < 100; i++)
    {
      if (max < particles[i].likehood)
        max = particles[i].likehood;
      buffer_parameter += particles[i].likehood;
    }
    std::cout << max << std::endl;
    std::cout << buffer_parameter << std::endl;
    // std::cout << (double)std::rand() / RAND_MAX << std::endl;
    for (int i = 0; i < 100; i++)
    {
      double sum = 0.0;
      double sum_2 = 0.0;
      int index;
      float rand = (double)std::rand() / RAND_MAX;
      for (int j = 0; j < 100; j++)
      {
        for (int k = 0; k < j+1; k++)
        {
          sum += particles[k].likehood;
          sum_2 += particles[k].likehood;
        }
        sum_2 += particles[j+1].likehood;
        if (sum < rand && sum_2 > rand)
          index = j + 1;
      }
      particles[i].x = particles[index].x;
      particles[i].y = particles[index].y;
    }
    for (int i = 0; i < 100; i++)
    {
      center_x += particles[i].x;
      particles[i].x += (std::rand() % 21 - 10);
      center_y += particles[i].y;
      particles[i].y += (std::rand() % 21 - 10);
      if (particles[i].x > 640)
        particles[i].x = 640;
      if (particles[i].x < 0)
        particles[i].x = 0;
      if (particles[i].y > 480)
        particles[i].y = 480;
      if (particles[i].y < 0)
        particles[i].y = 0;
      cv::circle(frame, cv::Point(particles[i].x, particles[i].y), 1, cv::Scalar(0,0,255), -1, CV_AA);
    }
    center_x /= 100;
    center_y /= 100;
    std::cout << center_x << std::endl;
    std::cout << center_y << std::endl;
    cv::imshow("win", frame);//画像を表示
    // cv::Mat hsv_frame;
    // cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
    // cv::imshow("hsv", hsv_frame);
    cv::moveWindow("win", 400, 400);
    const int key = cv::waitKey(1);
    if(key == 'q'/*113*/)//qボタンが押されたとき
    {
      break;//whileループから抜ける．
    }
    else if(key == 's'/*115*/)//sが押されたとき
    {
        //フレーム画像を保存する．
      cv::imwrite("img.png", frame);
    }
  }
  cv::destroyAllWindows();
  return 0;
}
