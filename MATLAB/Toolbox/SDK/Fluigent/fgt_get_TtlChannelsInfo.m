function [ infoArray ] = fgt_get_TtlChannelsInfo( )
%FGT_GET_TTLCHANNELSINFO Retrieve information about each initialized TTL channel. 
%
% infoArray = fgt_get_TtlChannelsInfo() returns a structure array
% containing the following fields for each TTL channel:
%
% ControllerSN: serial number of the controller
% firmware: firmware version
% DeviceSN: serial number of the channel, if applicable
% position: index of the channel within the controller
% index: global index of the channel across all instruments
% indexID: instrument's unique identifier
% InstrType: type of instrument
%
% This function is useful in order to get channels order, controller,
% unique ID and instrument type.
% TTL channels are only available for LineUP Series, 2 ports for each
% connected Link.


[error_code, infoArray]= LowLevel.fgt_get_TtlChannelsInfo();
for i=1:numel(infoArray)
    infoArray(i).InstrType = fgt_INSTRUMENT_TYPE(infoArray(i).InstrType);
end
manage_generic_status('fgt_get_TtlChannelsInfo', error_code);
end

