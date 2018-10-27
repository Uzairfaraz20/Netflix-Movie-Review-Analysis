# Netflix-Movie-Review-Analysis
## Overview
- This program analyzes Netflix movie reviews. There are 2 included files: one containing movie names and one contaning reviews
for those movies. The program performs two different anyalsis:
  1. Output the top 10 movies based on the reviews from the reviews file. The movies are sorted based on their score on the 5 star
  rating scale. If two movies have the same rating, the secondary sort is by name is in ascending order.
  2. Allows the user to enter a movie or review ID and if found, output the data associated with that particular movie or review.
  ID's are positive integers. Entering a negative ID will only result in an error message. Movie ID's are from 0 - 100,000 and Review ID's
  numbers greater than 100,000.
  
## Input files
- Input files are in the .csv file format.
- The  file **“movies.csv”** defines  a  set  of  movies,  in  alphabetical  order  by  movie  name.   
The  format  is  as  follows: **MovieID, MovieName, PubYear**.
- The file **"reviews.csv"** contains  movie  reviews,  in  order  by  review  date (with  secondary  sortby  user  id).
The  format  is  as  follows: **ReviewID, MovieID, UserID, Rating, ReviewDate**.
- **"reviews-2.csv"** is also included as a tester file.
