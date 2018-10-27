/*main.cpp*/

//
// Movie Analysis program in modern C++.
//
// << Uzair Mohammed >>
// U. of Illinois, Chicago
// CS 341: Fall 2018
// Project 02
//



#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <functional>
#include <list>
#include <map>
#include <chrono>


using namespace std;



////////////////// Classes /////////////////////////////////

//movie class
class Movie
{
private:
	int movieId;
	string movieName;
	int pubYear;
	int oneStars;
	int twoStars;
	int threeStars;
	int fourStars;
	int fiveStars;

public:
	
	//constructor
	Movie(int id, string name, int year) {
		movieId = id;
		movieName = name;
		pubYear = year;
		oneStars = 0;
		twoStars = 0;
		threeStars = 0;
		fourStars = 0;
		fiveStars = 0;
	}

	//getters
	int getMovieId() {return movieId;}
	string getMovieName() {return movieName;}
	int getPubYear() {return pubYear;}
	int getOneStars() { return oneStars; }
	int getTwoStars() { return twoStars; }
	int getThreeStars() { return threeStars; }
	int getFourStars() { return fourStars; }
	int getFiveStars() { return fiveStars; }

	//kind of setters
	void addToOneStar() { oneStars ++; }
	void addToTwoStar() { twoStars ++ ; }
	void addToThreeStar() { threeStars ++; }
	void addToFourStar() { fourStars ++; }
	void addToFiveStar() { fiveStars ++; }

	

};

// review class
class Review
{
private:
	int reviewId;
	int movieId;
	int userId;
	int rating;
	string reviewDate;

public:
	//constructor
	Review(int rId, int mId, int uId, int r, string date) {
		reviewId = rId;
		movieId = mId;
		userId = uId;
		rating = r;
		reviewDate = date;
	}

	//getters
	int getReviewId() { return reviewId; }
	int getMovieId() { return movieId; }
	int getUserId() { return userId; }
	int getRating() { return rating; }
	string getReviewDate() { return reviewDate; }

};

//class to make top ten display easier
class Ranking {
	
private:
	int movieId;
	string movieName;
	int totalReviews;
	double avgRating;

public:
	Ranking(int movId, string name, int total, double avg){
		movieId = movId;
		movieName = name;
		totalReviews = total;
		avgRating = avg;
	}

	int getMovieId() { return movieId; }
	string getMovieName() { return movieName; }
	int getTotalReviews() { return totalReviews; }
	double getAvgRating() { return avgRating; }
};

class ReviewMap {
	
private:
	map<int, vector<Review>> reviewByMovieMap;
	map<int, Review> reviewMap;

public:

	ReviewMap(map<int, vector<Review>> rbmm, map<int, Review> rm) {
		reviewByMovieMap = rbmm;
		reviewMap = rm;
	}

	map<int, vector<Review>> getReviewByMovieMap() { return reviewByMovieMap; }
	map<int, Review> getReviewMap() { return reviewMap; }

};




/////////////////// Utility functions ///////////////////////

//builds and populates a movie vector from file
map<int,Movie> buildMovie(ifstream& file) {
	
	string line;
	string movieId;
	string movieName;
	string pubYear;
	map<int, Movie> movies;

	getline(file, line);

	while (getline(file, line)) {
		
		stringstream ss(line);

		getline(ss, movieId, ',');
		getline(ss, movieName, ',');
		getline(ss, pubYear, ',');
		
		Movie newMovie(stoi(movieId), movieName, stoi(pubYear));

		movies.insert(pair<int, Movie>(newMovie.getMovieId(), newMovie));
	}

	return movies;

}

//builds and populates a review map<int, vector<Review>> from file;
ReviewMap buildReview(ifstream& file, map<int,Movie>& movies) {

	string line;
	string reviewId;
	string movieId;
	string userId;
	string rating;
	string reviewDate;
	map<int,vector<Review>> movieReviews;
	map<int, Review> reviews;

	getline(file, line);

	while (getline(file, line)) {
		
		stringstream ss(line);
		getline(ss, reviewId, ',');
		getline(ss, movieId, ',');
		getline(ss, userId, ',');
		getline(ss, rating, ',');
		getline(ss, reviewDate, ',');

		int mId = stoi(movieId);
		Review newReview(stoi(reviewId), mId, stoi(userId), stoi(rating), reviewDate);

		reviews.insert(pair<int, Review>(newReview.getReviewId(),newReview));

		auto movieIter = movies.find(mId);
		if (movieIter != movies.end()) {
			auto &movie = movieIter->second;


			switch (stoi(rating))
			{
			case 1:
				movie.addToOneStar();
				break;
			case 2:
				movie.addToTwoStar();
				break;
			case 3:
				movie.addToThreeStar();
				break;
			case 4:
				movie.addToFourStar();
				break;
			case 5:
				movie.addToFiveStar();
				break;
			}

		}
		
		
		auto result = movieReviews.find(newReview.getMovieId());

		if (result == movieReviews.end()) {
			vector<Review> reviews;
			reviews.push_back(newReview);
			movieReviews.insert(pair<int, vector<Review>>(newReview.getMovieId(), reviews ));
		}
		else {
			result->second.push_back(newReview);
		}
	}

	ReviewMap reviewMap(movieReviews, reviews);

	return reviewMap;
}

vector<double> getMovieRatings(int movieId, map<int, vector<Review>> movieReviews) {
	vector<double> ratings;

	auto result = movieReviews.find(movieId);
	
	if (result == movieReviews.end()) {
		ratings.push_back(0.0);
		return ratings;
	}
	
	vector<Review> reviews = result->second;

	for (Review& r: reviews) {
		ratings.push_back(r.getRating());
	}

	return ratings;
}

double getMovieRatingAvg(vector<double> ratings) {

	double sum = accumulate(ratings.begin(), ratings.end(), 0.0,
		[](double total, double curr) {
		return total + curr;
	}

	);

	double avg;
	if (ratings.size() > 0.0) {
		avg = sum / ratings.size();
	}
	else {
		avg = 0.0;
	}

	return avg;
}



/////////////// doPartX helper functions ///////////////////////

void doPart1(map<int, Movie> movies, map<int, vector<Review>> movieReviews) {

	vector<Ranking> rankings;

	for (auto  m : movies) {

		vector<double> ratings = getMovieRatings(m.second.getMovieId(), movieReviews);

		double avg = getMovieRatingAvg(ratings);

		Ranking newRanking(m.second.getMovieId(), m.second.getMovieName(), ratings.size(), avg);

		rankings.push_back(newRanking);

	}


	//main sort
	sort(rankings.begin(), rankings.end(),
		[](Ranking r1, Ranking r2) {
		
		if (r1.getAvgRating() > r2.getAvgRating()) {
			return true;
		}
		else if (r1.getAvgRating() == r2.getAvgRating()) {
			return r1.getMovieName() > r2.getMovieName();
		}
		
		else {
			
			return false;
		}

	}

	);



	cout << ">> Top-10 Movies <<" << endl;
	cout << endl;
	cout << "Rank\tID\tReviews\tAvg\tName" << endl;
	
	
	//output block
	int rank = 1;
	for (Ranking& r : rankings) {
		cout << rank << ".\t" << r.getMovieId() << "\t" << r.getTotalReviews()
			<< "\t" << r.getAvgRating() << "\t" << "'" << r.getMovieName() << "'" << endl;
		
		
		rank++;
		if (rank > 10) {
			break;
		}
	}

	



}


void doPart2ListMovie(map<int,Movie> movies, ReviewMap reviews, int idToSearch) {
	
	auto movieIter = movies.find(idToSearch);

	if (movieIter == movies.end()) {

		cout << "\n" << endl;
		cout << "movie not found..." << endl;
	}
	else {

		vector<double> ratings = getMovieRatings(idToSearch, reviews.getReviewByMovieMap());

		double avg = getMovieRatingAvg(ratings);
		int totaReviews = ratings.size();

		if (totaReviews == 1 && ratings.at(0) == 0.0) {
			totaReviews = 0;
		}

		cout << "\n" << endl;
		cout << "Movie:\t\t" << "'" << movieIter->second.getMovieName() << "'" << endl;
		cout << "Year:\t\t" << movieIter->second.getPubYear()  << endl;
		cout << "Avg rating:\t" <<avg  << endl;
		cout << "Num reviews:\t" << totaReviews << endl;
		cout << " 1 star: \t" << movieIter->second.getOneStars()  << endl;
		cout << " 2 star: \t" << movieIter->second.getTwoStars()  << endl;
		cout << " 3 star: \t" << movieIter->second.getThreeStars() << endl;
		cout << " 4 star: \t" << movieIter->second.getFourStars()  << endl;
		cout << " 5 star: \t" << movieIter->second.getFiveStars()  << endl;



	}

}

void doPart2ListReview(map<int, Review> reviewMap, map<int, Movie> movieMap, int idToSearch) {

	auto reviewIter = reviewMap.find(idToSearch);
	

	if (reviewIter == reviewMap.end()) {
		cout << "\n" <<endl;
		cout << "review not found..." << endl;
	}
	else {
		
		auto movieIter = movieMap.find(reviewIter->second.getMovieId());

		cout << endl;
		cout << "Movie: " <<reviewIter->second.getMovieId() << "\t" << "("<< movieIter->second.getMovieName() << ")" << endl;
		cout << "Num stars:\t" <<reviewIter->second.getRating() << endl;
		cout << "User id:\t"<<reviewIter->second.getUserId() << endl;
		cout << "Date:\t\t" <<reviewIter->second.getReviewDate() << endl;
	}

}





int main()
{
  
	cout << "** Netflix Movie Review Analysis **" << endl;
	cout << endl;

  cout << std::fixed;
  cout << std::setprecision(5);

  //read in movie and review files
  string  movieFile;
  string reviewFile;
  cin >> movieFile;
  cin >> reviewFile;
  
  //START TIMER
  auto  beginTime = std::chrono::steady_clock::now();

  //convert to ifstreams
  ifstream movFile(movieFile);
  ifstream revFile(reviewFile);
  
//----------------movie Work------------------------------------

  if (!movFile.good()) {
	  cout << "cannot  open  file!" << endl;
	  return  -1;
  }
 
  cout << endl;

  map<int, Movie> movies = buildMovie(movFile);


  
  
  cout << ">> Reading movies... " << movies.size();

  auto  movieBuildTime = std::chrono::steady_clock::now();
  auto movieDiff = movieBuildTime - beginTime;
  cout << "  [  time:  " << std::chrono::duration<double, std::milli>(movieDiff).count() << "  ms  ]" << endl;

  
 

  //--------------review work-------------------------------------

  if (!revFile.good()) {
	  cout << "cannot  open  file!" << endl;
	  return  -1;
  }
  
  cout << endl;



  ReviewMap reviews = buildReview(revFile, movies);

  


  cout << ">> Reading reviews ... " << reviews.getReviewMap().size();

  auto  reviewBuildTime = std::chrono::steady_clock::now();
  auto reviewDiff = reviewBuildTime - beginTime;
  cout << "  [  time:  " << std::chrono::duration<double, std::milli>(reviewDiff).count() << "  ms  ]" << endl;


  //--------------------------------------------------------------

  doPart1(movies, reviews.getReviewByMovieMap());
  
  auto  endTime = std::chrono::steady_clock::now();
  auto diff = endTime - beginTime;
  cout << "  [  time:  " << std::chrono::duration<double, std::milli>(diff).count() << "  ms  ]" << endl;


  cout << endl;
  cout << ">> Movie and Review Information <<"<<endl;

  //block tha reads input
  string idToSearch;
  while (idToSearch != "#") {
	  cout << endl;
	  cout << "Please enter a movie Id (< 100,000), a review Id (>= 100,000), 0 to stop> ";
	  cin >> idToSearch;
	 //exit
	  if (stoi(idToSearch) == 0) {
		  cout << "\n" << endl;
		  cout << "** DONE! **" << endl;
		  break;
	  }
	  //invalid id
	  if (stoi(idToSearch) < 0) {
		  cout << endl;
		  cout << "**invalid id..." << endl;
		  continue;
	  }

	  //search movies
	  if (stoi(idToSearch) < 100000) {
		  
		  doPart2ListMovie(movies, reviews, stoi(idToSearch));
		  
		//search reviews  
	  } else if (stoi(idToSearch) >= 100000) {
		  doPart2ListReview(reviews.getReviewMap(), movies, stoi(idToSearch));

	  }
	  cout << endl;
  }

  
  

  cout << endl;
  return 0;
}
