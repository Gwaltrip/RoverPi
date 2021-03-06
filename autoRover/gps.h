#ifndef GPS_HEADER
#define GPS_HEADER
struct TargetCords {
        double Longitude;
        double Latitude;
        struct TargetCords* Next;
        struct TargetCords* Back;
};
struct Gps {
	float(*GetError)(void);
	float(*GetLongitude)(void);
	float(*GetLatitude)(void);
	void(*UpdateThreadStart)(void);
	void(*UpdateThreadKill)(void);
	void(*GpsInit)(struct Gps*);
};
float GetError(void);
float GetLongitude(void);
float GetLatitude(void);
void UpdateThreadStart(void);
void UpdateThreadKill(void);
void UpdateGps(void* args);
void GpsInit(struct Gps* current);
#endif
