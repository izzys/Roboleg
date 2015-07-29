function [] = CB_save_data()
global h save_data
save_data= get(h.Save_Data_checkbox, 'Value');

