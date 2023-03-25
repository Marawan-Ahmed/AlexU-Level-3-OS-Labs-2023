#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>


void
station_init(struct station *station)
{
	// FILL ME IN

	// station->train_in_station = 0;
	pthread_mutex_init(&station->rsrc_lock, NULL);
	pthread_mutex_init(&station->waiting_lock, NULL);
	pthread_mutex_init(&station->seats_lock, NULL);
	pthread_mutex_init(&station->train_lock, NULL);
	pthread_mutex_init(&station->boarded_lock, NULL);


    pthread_cond_init(&station->train_in_station, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);

    pthread_mutex_lock(&station->waiting_lock);
    pthread_mutex_lock(&station->seats_lock);
	station->passengers_in_station = 0;
	station->empty_seats = 0;
    pthread_mutex_unlock(&station->waiting_lock);
    pthread_mutex_unlock(&station->seats_lock);

}

void
station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&station->train_lock);
	if (count == 0) {pthread_mutex_unlock(&station->train_lock); return;}
	if (station->passengers_in_station == 0) {pthread_mutex_unlock(&station->train_lock); return;}
	
	// FILL ME IN
    pthread_mutex_lock(&station->seats_lock);
	station->empty_seats = count;
	printf("seats: %d\n", station->empty_seats);
    pthread_mutex_unlock(&station->seats_lock);
    pthread_mutex_lock(&station->boarded_lock);
	station->boarded_passengers = 0;
    pthread_mutex_unlock(&station->boarded_lock);
	pthread_cond_broadcast(&station->train_in_station);

    pthread_mutex_lock(&station->boarded_lock);
	while(station->passengers_in_station > 0 && station->boarded_passengers < count)pthread_cond_wait(&station->passenger_boarded, &station->boarded_lock);
	pthread_mutex_unlock(&station->boarded_lock);
	
	pthread_mutex_unlock(&station->train_lock);
    return;	    
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
    pthread_mutex_lock(&station->waiting_lock);
	station->passengers_in_station++;
    pthread_mutex_unlock(&station->waiting_lock);

    pthread_mutex_lock(&station->seats_lock);
	while (station->empty_seats <= 0){
		pthread_cond_wait(&station->train_in_station, &station->seats_lock);
	}
	station->empty_seats --;
    pthread_mutex_unlock(&station->seats_lock);
		printf("passengers waiting: %d\n", station->empty_seats);

	return;
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	// printf("passengers waiting: %d\n", station->passengers_in_station);
	pthread_mutex_lock(&station->waiting_lock);
		station->passengers_in_station --;
    pthread_mutex_unlock(&station->waiting_lock);

    pthread_mutex_lock(&station->boarded_lock);
	station->boarded_passengers ++;
    pthread_mutex_unlock(&station->boarded_lock);
	pthread_cond_signal(&station->passenger_boarded);\
}
