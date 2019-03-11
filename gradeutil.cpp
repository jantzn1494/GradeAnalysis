/*gradeutil.cpp*/

//
// Nathan Jantz
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #02: GradeUtil API
// 

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "gradeutil.h"

using namespace std;


//
// API:
//

// 
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior 
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{
  std::stringstream csv(csvline);
  std::string info; //string used for info while parsing
  
  std::string  Dept;
  std::string  Title;
  int     Number;
  int     Section;
  std::string  Instructor;
  int     NumA, NumB, NumC, NumD, NumF, NumI, NumS, NumU, NumW, NumNR;
  
  std::getline(csv, info, ','); //get Dept info
  Dept = info;
  
  std::getline(csv, info, ','); //get Number info
  Number = std::stoi(info);
  
  std::getline(csv, info, ','); //get Section info
  Section = std::stoi(info);
  
  std::getline(csv, info, ','); //get  Title info
  Title = info;
  
  std::getline(csv, info, ','); //get NumA info
  NumA = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumB info
  NumB = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumC info
  NumC = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumD info
  NumD = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumF info
  NumF = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumI info
  NumI = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumNR info
  NumNR = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumS info
  NumS = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumU info
  NumU = std::stoi(info);
  
  std::getline(csv, info, ','); //get NumW info
  NumW = std::stoi(info);
  
  std::getline(csv, info, ','); //get Instructor info
  Instructor = info;
  
  return Course(Dept, Title, Number, Section, Instructor,
                NumA, NumB, NumC, NumD, NumF,
                NumI, NumS, NumU, NumW, NumNR);
}


//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course, 
// department, or college.  For a course whose grading type 
// is defined as Course::Letter, the DFW rate is defined as
// 
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.  
//
// When computed for a dept or college, all courses of type 
// Course::Letter are considered in computing an overall DFW 
// rate for the dept or college.  The reference parameters 
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  double percent = 0.0; //percent to be returned
  
  if(c.getGradingType() == Course::Letter){ //if course is of gradingType Letter
      DFW = c.NumD + c.NumF + c.NumW; //Numerator
      N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW; //Denominator
  }
  if(N > 0){
    percent = static_cast<double>(DFW) / N; //calculate percent
    percent = percent * 100.0;
  }
  return percent;
}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  double percent = 0.0; //percent to be returned
  
  for(auto c : dept.Courses){ //for each course in Courses vector
    if(c.getGradingType() == Course::Letter){ //if course is of GradingType Letter
      DFW = DFW + c.NumD + c.NumF + c.NumW; //Numerator
      N = N + c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW; //Denominator
    }
  }
  if(N > 0){
    percent = static_cast<double>(DFW) / N; //calculate percent
    percent = percent * 100.0;
  }
  return percent;
}

double GetDFWRate(const College& college, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  double percent = 0.0; //percent to be returned
  
  for(auto d : college.Depts){ //for each department in Depts vector
    for(auto c : d.Courses){ //for each course in Courses vector
      if(c.getGradingType() == Course::Letter){ //if course is of GradingType Letter
        DFW = DFW + c.NumD + c.NumF + c.NumW; //Numerator
        N = N + c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW; //Denominator
      }
    }
  }
  if(N > 0){
    percent = static_cast<double>(DFW) / N; //calculate percent
    percent = percent * 100.0;
  }
  return percent;
}


//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B, 
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall 
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c)
{
  int    N;  // total # of grades assigned:
  int    NumA, NumB, NumC, NumD, NumF; // number of A's, B's, et
  double PercentA, PercentB, PercentC, PercentD, PercentF;  // percentage of A's, B's, et
  
  N = NumA = NumB = NumC = NumD = NumF = 0;
  PercentA = PercentB = PercentC = PercentD = PercentF = 0.0;
  
  if(c.getGradingType() == Course::Letter){ //if c is of GradingType Letter, get grades
    N = N + c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
    NumA = NumA + c.NumA;
    NumB = NumB + c.NumB;
    NumC = NumC + c.NumC;
    NumD = NumD + c.NumD;
    NumF = NumF + c.NumF;
  }
  if(N > 0){ //If Number of total grades is greater than 0, get percentages
    if(NumA > 0){
      PercentA = NumA / static_cast<double>(N);
      PercentA = PercentA * 100.0;
    }
    if(NumB > 0){
      PercentB = NumB / static_cast<double>(N);
      PercentB = PercentB * 100.0;
    }
    if(NumC > 0){
      PercentC = NumC / static_cast<double>(N);
      PercentC = PercentC * 100.0;
    }
    if(NumD > 0){
      PercentD = NumD / static_cast<double>(N);
      PercentD = PercentD * 100.0;
    }
    if(NumF > 0){
      PercentF = NumF / static_cast<double>(N);
      PercentF = PercentF * 100.0;
    }
  }
  
  return GradeStats(N, NumA, NumB, NumC, NumD, NumF,
                    PercentA, PercentB, PercentC, PercentD, PercentF); 
}

GradeStats GetGradeDistribution(const Dept& dept)
{
  int    N;  // total # of grades assigned:
  int    NumA, NumB, NumC, NumD, NumF; // number of A's, B's, et
  double PercentA, PercentB, PercentC, PercentD, PercentF;  // percentage of A's, B's, et
  
  N = NumA = NumB = NumC = NumD = NumF = 0;
  PercentA = PercentB = PercentC = PercentD = PercentF = 0.0;
  
  for(auto c : dept.Courses){ //For each course in dept
    if(c.getGradingType() == Course::Letter){ //if c is of GradingType Letter, get grades
      N = N + c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
      NumA = NumA + c.NumA;
      NumB = NumB + c.NumB;
      NumC = NumC + c.NumC;
      NumD = NumD + c.NumD;
      NumF = NumF + c.NumF;
    }
  }
  if(N > 0){ //If Number of total grades is greater than 0, get percentages
    if(NumA > 0){
      PercentA = NumA / static_cast<double>(N);
      PercentA = PercentA * 100.0;
    }
    if(NumB > 0){
      PercentB = NumB / static_cast<double>(N);
      PercentB = PercentB * 100.0;
    }
    if(NumC > 0){
      PercentC = NumC / static_cast<double>(N);
      PercentC = PercentC * 100.0;
    }
    if(NumD > 0){
      PercentD = NumD / static_cast<double>(N);
      PercentD = PercentD * 100.0;
    }
    if(NumF > 0){
      PercentF = NumF / static_cast<double>(N);
      PercentF = PercentF * 100.0;
    }
  }
  
  return GradeStats(N, NumA, NumB, NumC, NumD, NumF,
                    PercentA, PercentB, PercentC, PercentD, PercentF); 
}

GradeStats GetGradeDistribution(const College& college)
{
  int    N;  // total # of grades assigned:
  int    NumA, NumB, NumC, NumD, NumF; // number of A's, B's, et
  double PercentA, PercentB, PercentC, PercentD, PercentF;  // percentage of A's, B's, et
  
  N = NumA = NumB = NumC = NumD = NumF = 0;
  PercentA = PercentB = PercentC = PercentD = PercentF = 0.0;
  
  for(auto d : college.Depts){ //For each dept in college
    for(auto c : d.Courses){ //For each course in dept
      if(c.getGradingType() == Course::Letter){ //if c is of GradingType Letter, get grades
        N = N + c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
        NumA = NumA + c.NumA;
        NumB = NumB + c.NumB;
        NumC = NumC + c.NumC;
        NumD = NumD + c.NumD;
        NumF = NumF + c.NumF;
      }
    }
  }
  if(N > 0){ //If Number of total grades is greater than 0, get percentages
    if(NumA > 0){
      PercentA = NumA / static_cast<double>(N);
      PercentA = PercentA * 100.0;
    }
    if(NumB > 0){
      PercentB = NumB / static_cast<double>(N);
      PercentB = PercentB * 100.0;
    }
    if(NumC > 0){
      PercentC = NumC / static_cast<double>(N);
      PercentC = PercentC * 100.0;
    }
    if(NumD > 0){
      PercentD = NumD / static_cast<double>(N);
      PercentD = PercentD * 100.0;
    }
    if(NumF > 0){
      PercentF = NumF / static_cast<double>(N);
      PercentF = PercentF * 100.0;
    }
  }
  
  return GradeStats(N, NumA, NumB, NumC, NumD, NumF,
                    PercentA, PercentB, PercentC, PercentD, PercentF); 
}


//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
  std::vector<Course>  courses; //vector of courses to be returned
  
  for(auto c : dept.Courses){ //for each course in Courses vector
    if(c.Number == courseNumber){ //if Number matches courseNumber
      courses.push_back(c); //add to courses vector
    }
  }
  
  if(courses.size() > 1){ //if there is more than one course
    //ascending order by section
    std::sort(courses.begin(), courses.end(), [](const Course& a, const Course& b){
      return a.Section < b.Section;
    });
  }
  
  return courses;
}


//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If 
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in 
// ascending order by course number.  If two courses have the 
// same course number, they are given in ascending order by 
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
  std::vector<Course>  courses; //vector of courses to be returned
  
  for(auto c : dept.Courses){ //for each course in Courses vector
    if(c.Instructor.compare(0, instructorPrefix.length(), instructorPrefix) == 0){ //if Instructor contains prefix given
      courses.push_back(c); //add to courses vector
    }
  }
  
  if(courses.size() > 1){ //if there is more than one course
    //ascending order by Number, then section
    std::sort(courses.begin(), courses.end(), [](const Course& a, const Course& b){
      if(a.Number == b.Number){
        return a.Section < b.Section;
      }
      return a.Number < b.Number;
    });
  }
  
  return courses;
}


//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber)
{
  std::vector<Course>  courses; //vector of courses to be returned
  
  for(auto d: college.Depts){ //for each dept in Depts vector
    for(auto c : d.Courses){ //for each course in Courses vector
      if(c.Number == courseNumber){ //if Number matches courseNumber
        courses.push_back(c); //add to courses vector
      }
    }
  }
  
  if(courses.size() > 1){ //if there is more than one course
    //ascending order by dept, then course number, then section
    std::sort(courses.begin(), courses.end(), [](const Course& a, const Course& b){
      if(a.Dept == b.Dept){
        if(a.Number == b.Number){
          return a.Section < b.Section;
        }
        return a.Number < b.Number;
      }
      return a.Dept < b.Dept;
    });
  }
  
  return courses;
}


//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by instructor, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
  std::vector<Course>  courses; //vector of courses to be returned
  
  for(auto d : college.Depts){ //for each dept in Depts vector
    for(auto c : d.Courses){ //for each course in Courses vector
      if(c.Instructor.compare(0, instructorPrefix.length(), instructorPrefix) == 0){ //if Instructor contains prefix given
        courses.push_back(c); //add to courses vector
      }
    }
  }
  
  if(courses.size() > 1){ //if there is more than one course
    //ascending order by instructor, then Number, then section
    std::sort(courses.begin(), courses.end(), [](const Course& a, const Course& b){
      if(a.Instructor == b.Instructor){
        if(a.Number == b.Number){
          return a.Section < b.Section;
        }
        return a.Number < b.Number;
      }
      return a.Instructor < b.Instructor;
    });
  }
  
  return courses;
}