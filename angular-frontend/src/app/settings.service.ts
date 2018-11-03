import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { MessageService } from './message.service';
import { Network } from './network';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';


@Injectable({
  providedIn: 'root'
})
export class SettingsService {
  //private settingsUrl = 'http://192.168.4.1:8080/api/settings';
  private settingsUrl = 'http://10.0.80.188:8080/api/settings';

  constructor( 
	private http: HttpClient,
	private messageService: MessageService
  ) { }

  getNetwork(): Observable<Network> {
    this.log( '[SettingsService] getNetwork()' );
	return this.http.get<Network>( this.settingsUrl + '/network' )
			.pipe(
				tap( network => this.log( '[SettingsService] fetched network settings' ) ),
				catchError( this.handleError('getNetwork', { mac: "undef", ssid: "n/a", wifi_pw: "", network_type: "unknown" }) )
			);
  }

  // Send updated network settings to device
  //
  updateNetwork( network:Network ):Observable<any> {
	let data = new FormData();

	data.append( 'ssid', network.ssid );
	if ( network.wifi_pw )
		data.append( 'wifi_pw', network.wifi_pw );

	return this.http.post( this.settingsUrl + '/network', data )
			.pipe(
				tap( _ => this.log( '[SettingsService] update network settings' ) ),
				catchError( this.handleError<any>('updateNetwork') )
			);
  }


  private log( message: string ) {
    this.messageService.add( `[Settings] ${message}` );
	}

	/**
	* Handle Http operation that failed.
	* Let the app continue.
	* @param operation - name of the operation that failed
	* @param result - optional value to return as the observable result
	*/
	private handleError<T> (operation = 'operation', result?: T) {
		return (error: any): Observable<T> => {

			// TODO: send the error to remote logging infrastructure
			console.error(error); // log to console instead

			// TODO: better job of transforming error for user consumption
			this.log(`${operation} failed: ${error.message}`);

			// Let the app keep running by returning an empty result.
			return of(result as T);
		};
	}

}
