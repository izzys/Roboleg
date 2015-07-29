function [] = CB_Browse()
global previous_data_string h

DialogTitle = 'gimme file !!';

[FileName,PathName,FilterIndex] = uigetfile('*.mat',DialogTitle);
previous_data_string = [PathName FileName ];
set(h.Load_previous_data_textbox,'string',FileName)

