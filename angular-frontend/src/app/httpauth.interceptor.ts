// Add basic auth headers to http requests
// https://medium.com/@ryanchenkie_40935/angular-authentication-using-the-http-client-and-http-interceptors-2f9d1540eb8

import { Injectable } from '@angular/core';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable()
export class HttpAuthInterceptor implements HttpInterceptor {

  intercept( request: HttpRequest<any>, next: HttpHandler ): Observable<HttpEvent<any>> {
    let authhash = window.btoa("admin:admin");
    request = request.clone({
	  setHeaders: {
        'Content-type': "application/json",
		Authorization: `Basic ${authhash}`,
      }
    });
    console.log("authorization: Basic " + authhash);

    return next.handle( request );
  }
}
