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
#define PI 3.1415926535
//const int b=140;const  int g=140;const int r=140;

using namespace std;
using namespace cv;

//int is_inside_which_triangle(pair<int,int> p);
//bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y);
//float area(int x1, int y1, int x2, int y2, int x3, int y3);
void display_points(vector<pair<int,int> > v);
void draw_points(vector<pair<int,int> > p);
void draw_triangle(int a);
void draw_line(pair<int,int> p1,pair<int,int> p2);
int is_inside_triangle(pair<int,int> p );
int is_inside_circumcircle(pair<int,int> p);
//bool check_free_points(pair<int,int> p1,pair<int,int> p2);
bool check_which_side(pair<int,int> p1,pair<int,int> p2, pair<int,int> a);
void draw_3new_tri(int i, pair<int,int> p);
//bool is_free(pair<int,int> p1,pair<int,int> p2, int i);
bool is_free(pair<int,int> p1,pair<int,int> p2,pair<int,int> a ,pair<int,int> b);
float calcu_Angle(pair<int,int> A, pair<int,int> B, pair<int,int> C);
int lengthSquare(pair<int,int> X, pair<int,int> Y);
void draw_new_tri(pair<int,int> a,pair<int,int> b,pair<int,int> c);
void swap_dia(pair<int,int> p,pair<int,int> v,pair<int,int> a,pair<int,int> b, int i);


vector<pair<int,int> > point;
vector<vector<pair<int,int> > > tri;
vector<vector<pair<int,int> > > lin;
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
        pair<int,int> p(x,y);
        point.push_back(p);
    }
    //sort(point.begin(), point.end());  // array gets sorted
    draw_points(point);      // points drawn in field
    display_points(point);  cout<<"points displayed"<<endl;

    vector<pair<int,int> >::iterator it=point.begin();
    int i=0, s=point.size()-1;
    int inside, in_circum;
    while(s--){
        i++;
        if(i==3) draw_triangle(0);
        if(i>3){
            //inside=is_inside_which_triangle(point[i-1]);
            //cout<<inside<<"++++\n";
            inside=is_inside_triangle(point[i-1]);
            cout<<inside<<"++\n";
            if(inside>=0){
                draw_3new_tri(inside,point[i-1]);
            }
            //if(!(1+inside))
            in_circum=is_inside_circumcircle(point[i-1]);
            cout<<in_circum<<"+++++\n";
            if(in_circum>=0){

            }
        }


    }
    cout<<"---DISPLAYING ALL TRIANGLES---"<<endl;
    for(int k=0; k<tri.size(); k++){
        display_points(tri[k]);
        cout<<"**"<<endl;
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
void draw_triangle(int i){
    draw_line(point[i],point[i+1]);
    draw_line(point[i+1],point[i+2]);
    draw_line(point[i+2],point[i]);
    vector<pair<int,int> > v;
    v.push_back(point[i]);
    v.push_back(point[i+1]);
    v.push_back(point[i+2]);
    //cout<<v[i+2].first<<" "<<v[i+2].second;
    tri.push_back(v);
    cout<<tri.size()<<"-- draw_triangle ends--"<<endl;
}
void draw_line(pair<int,int> p1,pair<int,int> p2){
    line(im,Point(p1.first,p1.second),Point(p2.first, p2.second), Scalar(0,0,255),2,8);
    vector<pair<int,int> > v;
    v.push_back(p1);
    v.push_back(p2);
    lin.push_back(v);
    cout<<lin.size()<<"\n";
    //display_points(v);
    cout<<"#### draw_line() end #####\n";
    //b=b+10; g=g+5; r=r-10;
  }
int is_inside_triangle(pair<int,int> p ){
    //cout<<p.first<<" "<<p.second;
    for(int i=0; i<tri.size(); i++){
      //for(int j=0; j<3; j++){
        vector<pair<int,int> > v;
        v=tri[i];
        bool t1,t2,t3;
        t1=check_which_side(v[0],v[1],p);
        t2=check_which_side(v[1],v[2],p);
        t3=check_which_side(v[2],v[0],p);
        if(t1==t2 && t2==t3)
            return i;
    }
    return -1;
  }
bool check_which_side(pair<int,int> p1,pair<int,int> p2, pair<int,int> a){
    int x1,x2,y2,y1,r1;
    x1=p1.first;
    y1=-p1.second;
    x2=p2.first;
    y2=-p2.second;
    r1=((-a.second-y1)*(x2-x1))-((a.first-x1)*(y2-y1));
    //r2=((p2.second-y1)*(x2-x1))-((p2.first-x1)*(y2-y1));
    if(r1 > 0 || r1==0) return true;
      return false;
  }
void draw_3new_tri(int i, pair<int,int> p){
    cout<<"inside draw_3new_tri()\n";
    vector<pair<int,int> > v;
    v=tri[i];
    vector<vector<pair<int,int> > >::iterator it=tri.begin();
    tri.erase((it+i));
    for(int j=0; j<3; j++){
      vector<pair<int,int> > v2;
      v2.push_back(p);
      v2.push_back(v[j]);
      if((j+1)==3) v2.push_back(v[0]);
      else v2.push_back(v[j+1]);
      tri.push_back(v2);
      draw_line(p,v[j]);
    }
}
int is_inside_circumcircle(pair<int,int> p){
  cout<<"inside is_inside_circumcircle\n";
    //cout<<p.first<<" "<<p.second;
    pair<int,int> vtx;
    for(int i=0; i<tri.size(); i++){
      vector<pair<int,int> > v=tri[i];
        bool t1=is_free(p,v[0],v[1],v[2]);
        bool t2=is_free(p,v[1],v[0],v[2]);
        bool t3=is_free(p,v[2],v[0],v[1]);
        if(t1==false) {
          vtx=v[0];
          if((calcu_Angle(v[1],vtx,v[2]) + calcu_Angle(v[1],p,v[2])) > 180 )
            { swap_dia(p,vtx,v[1],v[2],i);
            return i;}
        }
        else if(t2==false){
          vtx=v[1];
          if((calcu_Angle(v[0],vtx,v[2]) + calcu_Angle(v[0],p,v[2])) > 180 )
            { swap_dia(p,vtx,v[0],v[2],i);
            return i; }
        }
        else if(t3==false) {
          vtx=v[2];
          if((calcu_Angle(v[1],vtx,v[0]) + calcu_Angle(v[1],p,v[0])) > 180 )
            { swap_dia(p,vtx,v[0],v[1],i);
            return i; }
        }

      }//cout<<"\n^^^"<<vtx.first<<"  "<<vtx.second;



    return -1;
}
bool is_free(pair<int,int> p1,pair<int,int> p2,pair<int,int> a ,pair<int,int> b){
    int x1,x2,y1,y2,r1,r2;
        x1=a.first;
        y1=-a.second;
          x2=b.first;
          y2=-b.second;

        r1=((-p1.second-y1)*(x2-x1))-((p1.first-x1)*(y2-y1));
        r2=((-p2.second-y1)*(x2-x1))-((p2.first-x1)*(y2-y1));
        //cout<<"\n---"<<r1<<"  "<<r2;
        if(r1>0 && r2<0) return false;
        if(r1<0 && r2>0) return false;
          //cout<<"true\n";
          return true;

}

int lengthSquare(pair<int,int> X, pair<int,int> Y)
{
    int xDiff = X.first - Y.first;
    int yDiff = X.second - Y.second;
    return xDiff*xDiff + yDiff*yDiff;
}

float calcu_Angle(pair<int,int> A, pair<int,int> B,
                pair<int,int> C)
{
    // Square of lengths be a2, b2, c2
    int a2 = lengthSquare(B,C);
    int b2 = lengthSquare(A,C);
    int c2 = lengthSquare(A,B);

    // length of sides be a, b, c
    float a = sqrt(a2);
    float b = sqrt(b2);
    float c = sqrt(c2);

    // From Cosine law
    float alpha = acos((b2 + c2 - a2)/(2*b*c));
    float betta = acos((a2 + c2 - b2)/(2*a*c));
    float gamma = acos((a2 + b2 - c2)/(2*a*b));

    // Converting to degree
    alpha = alpha * 180 / PI;
    betta = betta * 180 / PI;
    gamma = gamma * 180 / PI;
    // cout<<"............."<<gamma<<endl;
    // cout<<"............."<<alpha<<endl;
    cout<<"............."<<betta<<endl;
    return betta;
}

void swap_dia(pair<int,int> p,pair<int,int> v,pair<int,int> a,pair<int,int> b, int i){
  vector<vector<pair<int,int> > >::iterator it=tri.begin();
  // remove_line(i); // to remove existing triangle
  tri.erase((it+i));
  vector<pair<int,int> > v1,v2;
  v1.push_back(p);
  v1.push_back(v);
  v1.push_back(a);
  tri.push_back(v1);
  draw_new_tri(p,v,a);
  v2.push_back(p);
  v2.push_back(v);
  v2.push_back(b);
  tri.push_back(v2);
  draw_new_tri(p,v,b);
}

void draw_new_tri(pair<int,int> a,pair<int,int> b,pair<int,int> c){
  line(im,Point(a.first,a.second),Point(b.first, b.second), Scalar(0,255,255),2,8);
  line(im,Point(c.first,c.second),Point(b.first, b.second), Scalar(0,255,255),2,8);
  line(im,Point(a.first,a.second),Point(c.first, c.second), Scalar(0,255,255),2,8);
  // b=b+10; g=g+5; r=r-10;
  // vector<pair<int,int> > v;
  // v.push_back(p1);
  // v.push_back(p2);
  // lin.push_back(v);
  // cout<<lin.size()<<"\n";
  // //display_points(v);
  // cout<<"#### draw_line() end #####\n";
}

// bool is_free(pair<int,int> p1,pair<int,int> p2, int i){
//     cout<<"inside is_free()";
//     int x1,y1,x2,y2, r1,r2;
//     cout<<lin.size()<<"^^^^^\n";
//     for(int j=0; j<3; j++){
//       x1=tri[i][j].first;
//       y1=tri[i][j].second;
//       if((j+1)==3){
//         x2=tri[i][0].first;
//         y2=tri[i][0].second;
//       } else{
//         x2=tri[i][j+1].first;
//         y2=tri[i][j+1].second;
//       }
//       r1=((p1.second-y1)*(x2-x1))-((p1.first-x1)*(y2-y1));
//       r2=((p2.second-y1)*(x2-x1))-((p2.first-x1)*(y2-y1));
//       if((r1*r2) > 0 || (r1*r2)==0) {cout<<"true\n";
//        continue; }
//       else{
//         //pair<int,int> a(x1,y1),b(x2,y2),c(p1.first,p1.second),d(p2.first,p2.second);
//         //if(check2(a,b,c,d)) continue;
//         cout<<"false\n";
//         return false; }
//     }
//     return true;
// }


// int is_inside_which_triangle(pair<int,int> p ){
//       cout<<p.first<<" "<<p.second<<endl;
//       for(int i=0; i<tri.size(); i++){
//         //for(int j=0; j<3; j++){
//           vector<pair<int,int> > v;
//           v=tri[i];
//           cout<<v[0].first<<" "<<v[0].second<<" "<<v[1].first<<" "<<v[1].second<<" "<<v[2].first<<" "<<v[2].second<<" ";
//
//           if(isInside(v[0].first,v[0].second,v[1].first,v[1].second,v[2].first,v[2].second,p.first,p.second))
//               return i;
//       }
//       return -1;
//     }

// bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
//   {
//      /* Calculate area of triangle ABC */
//      float A = area (x1, y1, x2, y2, x3, y3);
//
//      /* Calculate area of triangle PBC */
//      float A1 = area (x, y, x2, y2, x3, y3);
//
//      /* Calculate area of triangle PAC */
//      float A2 = area (x1, y1, x, y, x3, y3);
//
//      /* Calculate area of triangle PAB */
//      float A3 = area (x1, y1, x2, y2, x, y);
//
//      /* Check if sum of A1, A2 and A3 is same as A */
//      return (A == (A1 + A2 + A3));
//   }
// float area(int x1, int y1, int x2, int y2, int x3, int y3)
//   {
//      return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
//   }
