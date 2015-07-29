function CB_CloseGUI(src,callbackdata)
% Close request function 
% to display a question dialog box 
   selection = questdlg('Close This Figure?',...
      'Close Request Function',...
      'Yes','No','Yes'); 
   switch selection, 
      case 'Yes',
         CB_Stop_serial()
         delete(src)
      case 'No'
      return 
   end
end