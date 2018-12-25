#include "contentbridge.hpp"

void CContentBridge::run() {
	static bool cbridge = false;

	if (cbridge != true) {
		HRESULT test = URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/scout.png", "C:\\icons\\radar\\scout.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/sniper.png", "C:\\icons\\radar\\sniper.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/soldier.png", "C:\\icons\\radar\\soldier.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/engineer.png", "C:\\icons\\radar\\engineer.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/demoman.png", "C:\\icons\\radar\\demoman.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/pyro.png", "C:\\icons\\radar\\pyro.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/spy.png", "C:\\icons\\radar\\spy.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/medic.png", "C:\\icons\\radar\\medic.png", NULL, NULL);
		URLDownloadToFileA(NULL, "http://citadels.ddns.net/UI/icons/radar/heavy.png", "C:\\icons\\radar\\heavy.png", NULL, NULL);

		cbridge = true;
	}
}