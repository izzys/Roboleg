function [] = CB_load_previous_data()
global h load_previous_data previous_data_string
load_previous_data= get(h.Load_previous_data_checkbox, 'Value');
previous_data_string = get(h.Load_previous_data_textbox, 'string');

if ~load_previous_data
    set(h.Load_previous_data_textbox,'string','... \insert data path here ... Muthfaka!!!')
end
