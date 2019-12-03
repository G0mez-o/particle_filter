#include <particle_filter.hpp>
#include <boost/math/constants/constants.hpp>

const int PARTICLE_NUM = 350;
#define H_MAX 120
#define H_MIN 85
#define S_MAX 255
#define S_MIN 50
#define V_MAX 255
#define V_MIN 50
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
  for (int i = 0; i < PARTICLE_NUM; i++)
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
    // int center_x = particles[std::rand() % PARTICLE_NUM].x;
    // int center_y = particles[std::rand() % PARTICLE_NUM].y;
    double center_x;
    double center_y;
    double likehood_sum = 0.0;
    double buffer_parameter = 0.0;
    double max = 0.0;
    cv::Mat hsv_frame;
    cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
    cv::Mat mask;
    cv::Scalar s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
    cv::Scalar s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
    cv::inRange(hsv_frame, s_min, s_max, mask);
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
      cv::Vec3b color_buffer = frame.at<cv::Vec3b>(particles[i].y, particles[i].x);
      float color_dist = std::sqrt((color_buffer[0]) * (color_buffer[0]) + (color_buffer[1]) * (color_buffer[1])
                                   + (255 - color_buffer[2]) * (255 - color_buffer[2]));
      particles[i].likehood = std::exp(- color_dist * color_dist / 1800) / std::sqrt(3140);
      // cv::Vec3b color_buffer = mask.at<cv::Vec3b>(particles[i].y, particles[i].x);
      // float color_dist = (color_buffer[0] + color_buffer[1] + color_buffer[2]) / 3;
      // float color_dist = std::sqrt((255 - color_buffer[0]) * (255 - color_buffer[0]) + (255 - color_buffer[1]) * (255 - color_buffer[1])
      //                              + (255 - color_buffer[2]) * (255 - color_buffer[2]));
      // particles[i].likehood = std::exp(- color_dist * color_dist / 1000);

    }
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
      likehood_sum += particles[i].likehood;
    }
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
      particles[i].likehood /= likehood_sum;
    }
    // for (int i = 0; i < PARTICLE_NUM; i++)
    // {
    //   if (max < particles[i].likehood)
    //     max = particles[i].likehood;
    //   buffer_parameter += particles[i].likehood;
    // }
    // std::cout << max << std::endl;
    // std::cout << buffer_parameter << std::endl;
    // std::cout << (double)std::rand() / RAND_MAX << std::endl;
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
      double sum = 0.0;
      double sum_2 = 0.0;
      int index;
      float rand = (double)std::rand() / RAND_MAX;
      for (int j = 0; j < PARTICLE_NUM; j++)
      {
        for (int k = 0; k < j+1; k++)
        {
          sum += particles[k].likehood;
          sum_2 += particles[k].likehood;
        }
        sum_2 += particles[j+1].likehood;
        if (sum < rand && sum_2 > rand)
        {
          index = j + 1;
          break;
        }
          // index = j + 1;
      }
      particles[i].x = particles[index].x;
      particles[i].y = particles[index].y;
    }
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
      std::random_device seed_gen;
      std::default_random_engine engine(seed_gen());
      // 平均0.0、標準偏差1.0で分布させる
      std::normal_distribution<> dist(0, 15.0);
      double rand = dist(engine);
      double rand2 = dist(engine);
      // std::cout << rand << std::endl;
      // std::cout << rand2 << std::endl;
      // double buffer_x = (particles[i].x * particles[i].likehood);
      // std::cout << buffer_x << std::endl;
      center_x += particles[i].x;
      particles[i].x += rand;
      // double buffer_y = (particles[i].y * particles[i].likehood);
      center_y += particles[i].y;
      particles[i].y += rand2;
      if (particles[i].x > 640)
        particles[i].x = 640;
      if (particles[i].x < 0)
        particles[i].x = 0;
      if (particles[i].y > 480)
        particles[i].y = 480;
      if (particles[i].y < 0)
        particles[i].y = 0;
      cv::circle(frame, cv::Point(particles[i].x, particles[i].y), 1, cv::Scalar(0,255,0), -1, CV_AA);
    }
    center_x /= PARTICLE_NUM;
    center_y /= PARTICLE_NUM;
    cv::circle(frame, cv::Point((int)center_x, (int)center_y), 10, cv::Scalar(255,0,0), -1, CV_AA);
    std::cout << center_x << std::endl;
    std::cout << center_y << std::endl;
    // cv::Mat mask;
    // cv::Scalar s_min = cv::Scalar(H_MIN, S_MIN, V_MIN);
    // cv::Scalar s_max = cv::Scalar(H_MAX, S_MAX, V_MAX);
    // cv::inRange(hsv_frame, s_min, s_max, mask);
    // cv::Vec3b buffer = mask.at<cv::Vec3b>(0, 0);
    // std::cout << buffer << std::endl;
    cv::imshow("win", frame);//画像を表示
    // cv::Mat hsv_frame;
    // cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
    // cv::imshow("hsv", hsv_frame);
    cv::imshow("mask", mask);
    cv::moveWindow("win", 200, 200);
    // cv::moveWindow("hsv", 600, 600);
    cv::moveWindow("mask", 1000, 200);
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
