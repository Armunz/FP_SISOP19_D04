#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

char buffer[100];	//buat nampung data di crontab.data
char direktori[100];	//buat nampung direktori
char bintang1[3], bintang2[3], bintang3[3], bintang4[3], bintang5[3];

void *get_data(){
	char ch;
	int count;
	memset(buffer, '\0', sizeof(buffer));
	FILE *fp = fopen("crontab.data", "r");
	fseek(fp, 0L, SEEK_END);
     	count = ftell(fp);
     	fseek(fp, 0L, SEEK_SET);
	int i = 0;	
	while(count--){
		ch = fgetc(fp);
		buffer[i] = ch;
		i++;
	}
	
	char* token;
	char buffer2[100];
	memset(direktori, '\0', sizeof(direktori));
	memset(buffer2, '\0', sizeof(buffer2));
	
	memset(bintang1, '\0', sizeof(bintang1));
	memset(bintang2, '\0', sizeof(bintang2));
	memset(bintang3, '\0', sizeof(bintang3));
	memset(bintang4, '\0', sizeof(bintang4));
	memset(bintang5, '\0', sizeof(bintang5));

	strcpy(buffer2, buffer); 
    	char* rest = buffer2;
	int j=0;
	i=0;
    	while ((token = strtok_r(rest, " ", &rest))){ 
		if(j < 5){		
			if(j == 0){
				strcpy(bintang1, token);
			} 
			else if(j == 1){
				strcpy(bintang2, token);
			}
			else if(j == 2){
				strcpy(bintang3, token);
			}
			else if(j == 3){
				 strcpy(bintang4, token);
			}
			else if(j == 4){
				strcpy(bintang5, token);
			}
		}
		else{
			strcpy(direktori, token);
		}
		j++; 
	}
}

//brute force 32 kemungkinan
void *fungsi_cron(){
	time_t sekarang = time(NULL);
	struct tm waktu = *localtime(&sekarang);
	//printf("jam: %d:%d:%d\n", waktu.tm_hour, waktu.tm_min, waktu.tm_sec);

	//semua '*'
	char *bintang = "*";
	if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
		system(direktori);
		sleep(60);
	}
	
	//semua bukan '*'
	if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		int menit, jam, hari, bulan, nama_hari;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		hari = atoi(bintang3);
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);
		
		if(menit < 60 && jam < 24 && hari < 32 && bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_mday == hari && waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}

	//Empat '*' //1
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){		
		int jam = atoi(bintang2);
		if(jam < 24){
			if(waktu.tm_hour == jam){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//2
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
		int hari = atoi(bintang3);
		if(hari < 32){
			if(waktu.tm_mday == hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//3
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		int bulan = atoi(bintang4);
		if(bulan < 13){
			if(waktu.tm_mon == bulan){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//4
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){
		int nama_hari = atoi(bintang5);
		if(nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_wday == nama_hari){
				system(direktori);
				sleep(60);
			}
		}
		
		else{
			printf("Crontab salah format\n");
		}
	}
	//5
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
		int menit = atoi(bintang1);
		if(menit < 60){
			if(waktu.tm_min == menit){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	
	//satu '*' //1
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		
		int menit, jam, hari, bulan;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		hari = atoi(bintang3);
		bulan = atoi(bintang4);

		if(menit < 60 && jam < 24 && hari < 32 && bulan < 13){
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_mday == hari && waktu.tm_mon == bulan){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//2
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		
		int jam, hari, bulan, nama_hari;
		jam = atoi(bintang2);
		hari = atoi(bintang3);
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);

		if(jam < 24 && hari < 32 && bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_hour == jam && waktu.tm_mday == hari && waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//3
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		
		int menit, hari, bulan, nama_hari;
		menit = atoi(bintang1);
		hari = atoi(bintang3);
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);

		if(menit < 60 && hari < 32 && bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_mday == hari && waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}

	//4
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){

		int menit, jam, bulan, nama_hari;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);

		if(menit < 60 && jam < 24 && bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}

	//5
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){
		
		int menit, hari, jam, nama_hari;
		menit = atoi(bintang1);
		hari = atoi(bintang3);
		jam = atoi(bintang2);
		nama_hari = atoi(bintang5);

		if(menit < 60 && hari < 32 && jam < 24 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_mday == hari && waktu.tm_hour == jam && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}

	//tiga * //1
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		
		int bulan, nama_hari;
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);

		if(bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//2
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){

		int hari, bulan;
		hari = atoi(bintang3);
		bulan = atoi(bintang4);
		
		if(hari < 32 && bulan < 13){
			if(waktu.tm_mday == hari && waktu.tm_mon == bulan){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//3
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
	
		int jam, hari;
		jam = atoi(bintang2);
		hari = atoi(bintang3);
		
		if(jam < 24 && hari < 32){
			if(waktu.tm_hour == jam && waktu.tm_mday == hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}

	//4
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
		
		int menit, jam;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		
		if(menit < 60 && jam < 24){
			if(waktu.tm_min == menit && waktu.tm_hour == jam){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//5
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){
		
		int menit, nama_hari;
		menit = atoi(bintang1);
		nama_hari = atoi(bintang5);

		if(menit < 60 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}

	}
	//6
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){
		
		int menit, hari;
		menit = atoi(bintang1);
		hari = atoi(bintang3);
		
		if(menit < 60 && hari < 32){
			if(waktu.tm_min == menit && waktu.tm_mday == hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//7
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		
		int jam, bulan;
		jam = atoi(bintang2);
		bulan = atoi(bintang4);

		if(jam < 24 && bulan < 13){
			if(waktu.tm_hour == jam && waktu.tm_mon == bulan){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//8
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){
		
		int hari, nama_hari;
		hari = atoi(bintang3);
		nama_hari = atoi(bintang5);

		if(hari < 32 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_mday == hari && waktu.tm_wday == nama_hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//9
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){

		int menit, bulan;
		menit = atoi(bintang1);
		bulan = atoi(bintang4);

		if(menit < 60 && bulan < 13){
			if(waktu.tm_min == menit && waktu.tm_mon == bulan){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//10
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){
	
		int jam, nama_hari;
		jam = atoi(bintang2);
		nama_hari = atoi(bintang5);

		if(jam < 24 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_hour == jam && waktu.tm_wday == nama_hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}

	//Dua '*' //1
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) == 0){

		int menit, jam, hari;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		hari = atoi(bintang3);

		if(menit < 60 && jam < 24 && hari < 32){
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_mday == hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//2
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		
		int bulan, jam, hari;
		bulan = atoi(bintang4);
		jam = atoi(bintang2);
		hari = atoi(bintang3);

		if(bulan < 13 && jam < 24 && hari < 32){
			if(waktu.tm_mon == bulan && waktu.tm_hour == jam && waktu.tm_mday == hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//3
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		
		int nama_hari, bulan, hari;
		nama_hari = atoi(bintang5);
		bulan = atoi(bintang4);
		hari = atoi(bintang3);

		if(nama_hari < 8 && bulan < 13 && hari < 32){
			if(nama_hari == 7) nama_hari = 0;			
			if(waktu.tm_wday == nama_hari && waktu.tm_mon == bulan && waktu.tm_mday == hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//4
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		
		int menit, jam, bulan;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		bulan = atoi(bintang4);

		if(menit < 60 && jam < 24 && bulan < 13){
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_mon == bulan){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//5
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){
		
		int bulan, jam, nama_hari;
		bulan = atoi(bintang4);
		jam = atoi(bintang2);
		nama_hari = atoi(bintang5);

		if(bulan < 13 && jam < 24 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_mon == bulan && waktu.tm_hour == jam && waktu.tm_wday == nama_hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//6
	else if(strcmp(bintang1, bintang) == 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){		
		
		int nama_hari, jam, hari;
		nama_hari = atoi(bintang5);
		jam = atoi(bintang2);
		hari = atoi(bintang3);

		if(nama_hari < 8 && jam < 24 && hari < 32){
			if(nama_hari == 7) nama_hari == 0;
			if(waktu.tm_wday == nama_hari && waktu.tm_hour == jam && waktu.tm_mday == hari){
				system(direktori);
				sleep(60);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//7
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) == 0){
		
		int menit, bulan, hari;
		menit = atoi(bintang1);
		bulan = atoi(bintang4);
		hari = atoi(bintang3);

		if(menit < 60 && bulan < 13 && hari < 32){
			if(waktu.tm_min == menit && waktu.tm_mon == bulan && waktu.tm_mday == hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//8
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) != 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){

		int menit, nama_hari, hari;
		menit = atoi(bintang1);
		nama_hari = atoi(bintang5);
		hari = atoi(bintang3);

		if(menit < 60 && nama_hari < 8 && hari < 32){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_wday == nama_hari && waktu.tm_mday == hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//9
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) == 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) != 0 && strcmp(bintang5, bintang) != 0){

		int menit, bulan, nama_hari;
		menit = atoi(bintang1);
		bulan = atoi(bintang4);
		nama_hari = atoi(bintang5);

		if(menit < 60 && bulan < 13 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_mon == bulan && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	//10
	else if(strcmp(bintang1, bintang) != 0 && strcmp(bintang2, bintang) != 0 && strcmp(bintang3, bintang) == 0 && strcmp(bintang4, bintang) == 0 && strcmp(bintang5, bintang) != 0){

		int menit, jam, nama_hari;
		menit = atoi(bintang1);
		jam = atoi(bintang2);
		nama_hari = atoi(bintang5);

		if(menit < 60 && jam < 24 && nama_hari < 8){
			if(nama_hari == 7) nama_hari = 0;
			if(waktu.tm_min == menit && waktu.tm_hour == jam && waktu.tm_wday == nama_hari){
				system(direktori);
			}
		}
		else{
			printf("Crontab salah format\n");
		}
	}
	else{
		printf("Error anjay\n");
	}
}

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/nanta/Downloads/FP-sisop")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    	// main program here
    	pthread_t thread1, thread2;//inisialisasi awal
  	int  iret1, iret2;

  	iret1 = pthread_create( &thread1, NULL, get_data, NULL); //membuat thread pertama
  	if(iret1) //jika eror
  	{
        	fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        	exit(EXIT_FAILURE);
  	}
   	pthread_join(thread1, NULL);
  	iret2 = pthread_create( &thread1, NULL, fungsi_cron, NULL); //membuat thread pertama
  	if(iret1) //jika eror
  	{
        	fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        	exit(EXIT_FAILURE);
  	}
	pthread_join(thread2, NULL);
   
  }
  
  exit(EXIT_SUCCESS);
}
