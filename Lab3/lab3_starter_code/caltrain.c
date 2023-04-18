#include <pthread.h>
#include "caltrain.h"


void
station_init(struct station *station)
{
	pthread_mutex_init(&station->station_lock, NULL);

    pthread_cond_init(&station->train_in_station, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);

	pthread_mutex_lock(&station->station_lock);
		station->passengers_in_station = 0;
	pthread_mutex_unlock(&station->station_lock);	
    
	pthread_mutex_lock(&station->station_lock);	
		station->empty_seats = 0;
	pthread_mutex_unlock(&station->station_lock);	
}

void
station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&station->station_lock);	
		if (count == 0) {
			pthread_mutex_unlock(&station->station_lock);	
			return;
			}
		if (station->passengers_in_station == 0) {
			pthread_mutex_unlock(&station->station_lock);	
			return
			;}
		
		station->empty_seats = count;

		station->boarded_passengers = 0;

		pthread_cond_broadcast(&station->train_in_station);

		while(station->passengers_in_station > 0 && station->boarded_passengers < count) pthread_cond_wait(&station->passenger_boarded, &station->station_lock);

	pthread_mutex_unlock(&station->station_lock);	
    return;	    
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->station_lock);	
    
		station->passengers_in_station++;
    	while (station->empty_seats <= 0){
			pthread_cond_wait(&station->train_in_station, &station->station_lock);
		}
		station->empty_seats --;

	pthread_mutex_unlock(&station->station_lock);	

	return;
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->station_lock);	
		station->passengers_in_station --;
    	station->boarded_passengers ++;
    
	pthread_cond_signal(&station->passenger_boarded);
	pthread_mutex_unlock(&station->station_lock);	
}