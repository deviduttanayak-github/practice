#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
#include<fstream>
#include<iterator>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include<sstream>

//#include<pair>
using namespace std;
using namespace cv;

//bool draw_line(vector<pair> lin);
//bool check_intersection(vector<vector <pair>> lines, vector<pair> lines);
void display_points(vector<pair<int,int> > v);
void draw_points(vector<pair<int,int> > p);
bool check_free_points(pair<int,int> p1,pair<int,int> p2);
bool check2(pair<int,int> a,pair<int,int> b,pair<int,int> c,pair<int,int> d);
void draw_line(pair<int,int> p1,pair<int,int> p2);



vector<vector<pair<int,int> > > tri;
vector<vector<pair<int,int> > > lin;
vector<pair<int,int> > point;
Mat im;
int main(){
    Mat src=imread(("/home/devidutta/catkin_ws/src/delaunay/img.png"),IMREAD_COLOR);
    //imshow("Delaunay",src);
    im=src.clone();
    //vector
    ifstream f("/home/devidutta/catkin_ws/src/delaunay/points.txt", ios::in);
    while(!f.eof()){
        int x,y;
        f>>x>>y;
        pair<int, int> p(x,y);
        point.push_back(p);
    }
    sort(point.begin(), point.end());  // array gets sorted
    draw_points(point);      // points drawn in field
    display_points(point);  cout<<endl;
    vector<pair<int,int> >::iterator it=point.begin();
    int i=0, s=point.size()-1;
    while(s--){
      bool t=true;
      i++;
      vector<pair<int,int> >::iterator itt=point.begin();
      if(i>1){
        for(int j=0; j<i-1; j++){
          if(check_free_points(*it,*itt)){
              draw_line(*it,*itt);
          }
          itt++;
        }


      }
      imshow("Delaunay2",im);
      char k=waitKey(0);
      if(k=='m') cout<<"checked"<<endl;
      else while(1) {
        ;/* code */
      }
      it++;
    }

    imshow("Delaunay2",im);
    waitKey();
    return 0;
}

void display_points(vector<pair<int,int> > v){
    vector<pair<int,int> >::iterator it=v.begin();
    for(int i=0; i<v.size(); i++){
        cout<<(*it).first<<"  "<<(*it).second<<endl;
        it++;
    }
}
void draw_points(vector<pair<int,int> > p){
    vector<pair<int,int> >::iterator it=p.begin();
    for(int i=0; i<p.size()-1; i++){
      //point((*it).first, (*it).second);
      circle(im,Point((*it).first, (*it).second),10, Scalar(255,0,0),4,8);
      it++;
  }
}

bool check_free_points(pair<int,int> p1,pair<int,int> p2){
    //cout<<"function called";
    if(lin.size()==0) return true;
    int x1,y1,x2,y2, r1,r2;
    cout<<lin.size()<<"^^^^^\n";
    for(int i=0; i<lin.size(); i++){
      x1=lin[i][0].first;
      y1=lin[i][0].second;
      x2=lin[i][1].first;
      y2=lin[i][1].second;
      r1=((p1.second-y1)*(x2-x1))-((p1.first-x1)*(y2-y1));
      r2=((p2.second-y1)*(x2-x1))-((p2.first-x1)*(y2-y1));
      if((r1*r2) > 0 || (r1*r2)==0) {cout<<"true\n";
       continue; }
      else{
        pair<int,int> a(x1,y1),b(x2,y2),c(p1.first,p1.second),d(p2.first,p2.second);
        if(check2(a,b,c,d)) continue;
        cout<<"false\n";
        return false; }
    }
    return true;
}
bool check2(pair<int,int> a,pair<int,int> b,pair<int,int> c,pair<int,int> d){
  int x1,x2,y1,y2,r1,r2;
  x1=c.first;
  y1=c.second;
  x2=d.first;
  y2=d.second;
  r1=((a.second-y1)*(x2-x1))-((a.first-x1)*(y2-y1));
  r2=((b.second-y1)*(x2-x1))-((b.first-x1)*(y2-y1));
  if((r1*r2) > 0 || (r1*r2)==0) return true;
  return false;

}

void draw_line(pair<int,int> p1,pair<int,int> p2){
    line(im,Point(p1.first,p1.second),Point(p2.first, p2.second), Scalar(0,0,255),2,8);
    vector<pair<int,int> > v;
    v.push_back(p1);
    v.push_back(p2);
    lin.push_back(v);
    cout<<lin.size()<<"\n";
    display_points(v);
    cout<<"#########\n";
    /*char key=waitKey(9999999999);
    if(key=='l'){
      cout<<"checked\n";
    } else{
      waitKey(0);
    }*/
}
