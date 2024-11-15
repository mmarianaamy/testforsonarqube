/*
 * SonarQube, open source software quality management tool.
 * Copyright (C) 2008-2013 SonarSource
 * http://github.com/SonarOpenCommunity/sonar-cxx
 *
 * SonarQube is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * SonarQube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>
using namespace std;

struct Point
{
    int x, y;
};
Point p0;

Point nextToTop(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

void swap(Point &p1, Point &p2)
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

int distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}

int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

int compare(const void *vp1, const void *vp2)
{
   Point *p1 = (Point *)vp1;
   Point *p2 = (Point *)vp2;

   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

   return (o == 2)? -1: 1;
}

void convexHull(vector<Point> points, int n)
{
   int ymin = points[0].y, min = 0;
   for (int i = 1; i < n; i++)
   {
     int y = points[i].y;

     if ((y < ymin) || (ymin == y &&
         points[i].x < points[min].x)){
            ymin = points[i].y, min = i;
         }
   }

   swap(points[0], points[min]);

   p0 = points[0];
   qsort(&points[1], n-1, sizeof(Point), compare);

   int m = 1;
   for (int i=1; i<n; i++){
        while (i < n-1 && orientation(p0, points[i], points[i+1]) == 0){
            i++;
        }
        points[m] = points[i];
        m++;
    }

   if (m < 3) return;

   stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);

   for (int i = 3; i < m; i++)
   {
      while (S.size()>1 && orientation(nextToTop(S), S.top(), points[i]) != 2){
        S.pop();
      }
      S.push(points[i]);
   }

    vector<Point> result;
   while (!S.empty())
   {
       result.insert(result.begin(), S.top()); 
       S.pop();
   }

   for(Point i : result){
    cout << i.x << " " << i.y << endl;
   }
}

int main()
{
    int num, x, y;
    cin >> num;
    vector<Point> points;
    for(int i = 0; i < num; i++){
        cin >> x >> y;
        Point p;
        p.x = x;
        p.y = y;
        points.push_back(p);
    }

    convexHull(points, num);
    return 0;
}
