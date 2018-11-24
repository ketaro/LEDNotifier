import { BrowserModule }      from '@angular/platform-browser';
import { NgModule }           from '@angular/core';
import { FormsModule }        from '@angular/forms';
import { HttpClientModule,
         HTTP_INTERCEPTORS }   from '@angular/common/http';
import { HttpAuthInterceptor } from './http.interceptor';

import { AppComponent }       from './app.component';
import { NetworkComponent }   from './network/network.component';
import { MessagesComponent }  from './messages/messages.component';
import { AppRoutingModule }   from './app-routing.module';
import { DashboardComponent } from './dashboard/dashboard.component';
import { LedComponent }       from './led/led.component';
import { PaletteComponent } from './palette/palette.component';
import { PaletteDetailComponent } from './palette-detail/palette-detail.component';

@NgModule({
  declarations: [
    AppComponent,
    NetworkComponent,
    MessagesComponent,
    DashboardComponent,
    LedComponent,
    PaletteComponent,
    PaletteDetailComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [{
    provide: HTTP_INTERCEPTORS,
    useClass: HttpAuthInterceptor,
    multi: true
  }],
  bootstrap: [AppComponent]
})
export class AppModule { }
