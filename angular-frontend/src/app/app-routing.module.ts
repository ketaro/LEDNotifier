import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { DashboardComponent } from './dashboard/dashboard.component';
import { LedComponent } from './led/led.component';
import { NetworkComponent } from './network/network.component';

const routes: Routes = [
  { path: 'dashboard', component: DashboardComponent },
  { path: 'led', component: LedComponent },
  { path: 'network', component: NetworkComponent },
  { path: '', redirectTo: '/dashboard', pathMatch: 'full' },
];

@NgModule({
  imports: [ RouterModule.forRoot( routes ) ],
  exports: [ RouterModule ],
})
export class AppRoutingModule { }
