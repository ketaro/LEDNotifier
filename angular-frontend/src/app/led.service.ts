import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { MessageService } from './message.service';
import { HttpClient, HttpHeaders, HttpParams } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { LED } from './led';

@Injectable({
	providedIn: 'root'
})
export class LEDService {
	private serviceUrl = 'http://10.0.80.188:8080/api/led';

	constructor(
		private http: HttpClient,
		private messageService: MessageService
	) { }


	getLEDs(): Observable<LED> {
		this.log( '[LEDService] getLEDs()' );
		return this.http.get<LED>( this.serviceUrl + '/display' )
				.pipe(
					tap( led => this.log( '[LEDService] fetched LED display' ) ),
					catchError( this.handleError( 'getLEDs', { leds: 0, fps: 0 } ) )
				);
	}

	display( mode: number, duration: number ):void {
		let data = new FormData();
		data.append( 'mode', mode.toString() );
		data.append( 'duration', duration.toString() );

		this.log( "Setting display" );
		this.http.post( this.serviceUrl + '/display', data )
			.subscribe(
				response => console.log( response ),
				err      => console.log( err )
			);
//			.pipe(
//				tap( _ => this.log( 'set display' ) ),
//				catchError( this.handleError<any>('display') )
//			);
	}

	showAlert(): void {
		this.display( 6, 10 );
	}

	private log( message: string ) {
		this.messageService.add( `[LED] ${message}` );
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
