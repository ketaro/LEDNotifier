import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { PaletteDetailComponent } from './palette-detail.component';

describe('PaletteDetailComponent', () => {
  let component: PaletteDetailComponent;
  let fixture: ComponentFixture<PaletteDetailComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ PaletteDetailComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(PaletteDetailComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
