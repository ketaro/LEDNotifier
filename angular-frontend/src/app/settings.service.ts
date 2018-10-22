import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { MessageService } from './message.service';
import { Network } from './network';
import { HttpClient, HttpHeaders } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class SettingsService {
  private settingsUrl = 'http://192.168.4.1:8080/api/settings';

  constructor( 
	private http: HttpClient,
	private messageService: MessageService
  ) { }

  getNetwork(): Observable<Network> {
    this.log( 'fetched network settings' );
    return this.http.get<Network>( this.settingsUrl + '/network' );
  }

  private log( message: string ) {
    this.messageService.add( '[Settings] ${message}' );
  }

}
