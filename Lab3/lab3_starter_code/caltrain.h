#include <pthread.h>

struct station {
	// FILL ME IN
	int passengers_in_station;
	int empty_seats;
	int boarded_passengers;

	pthread_cond_t train_in_station;
	pthread_cond_t passenger_boarded;

	pthread_mutex_t waiting_lock;
	pthread_mutex_t seats_lock;
	pthread_mutex_t boarded_lock;
	
	pthread_mutex_t train_lock;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);