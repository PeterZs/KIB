var Widget = function(heading, kidgetname, properties, type) {
    if (!type) type = 'widget';
    var container_id = kidgetname + "_settings";
    var SettingsStr = "<div class='widgetsettings' id='" + container_id + "'>";
    for (var i = 0; i < properties.length; ++i) {
        SettingsStr += properties[i].getHtml();
    }
    SettingsStr += "</div>"

    var activate = function () {
        for (var i = 0; i < properties.length; ++i) {
            properties[i].update();
        }
    };
    var load = function(kiblet) {
        if (kidgetname in kiblet) {
            for (var i = 0; i < properties.length; ++i) {
                properties[i].update(kiblet[kidgetname][properties[i].id]);
            }
        }
    };
    var save = function(kiblet) {
        kiblet[kidgetname] = {};
        for (var i = 0; i < properties.length; ++i) {
            kiblet[kidgetname][properties[i].id] = properties[i].getValue();
        }
    };
    var reset = function(kiblet) {
        kiblet[kidgetname] = {};
    };
    var remove = function(kiblet) {
        delete kiblet[kidgetname];
    };
    return {
        type: type,
        name: kidgetname,
        settingsstr: SettingsStr, containerId: container_id, title: heading, activate: activate,
            loadSettings: load, saveSettings: save, reset: reset, remove: remove,
    };
}

var ArcWidget = Widget('Arc', 'arc', [
        SliderProperty('arc_discrete_slider', 'Discreteness', 0, 6, function(event,ui) {
            $('#arc_discrete_slider_label').html(ui.value==0?"Continuous":ui.value);
        }),
        SelectProperty('arc_visualization', 'Visualization',
                [{id:'arc_vis_button',label:'Buttons'},
                 {id:'arc_vis_arc',label:'Arc'},
                 {id:'arc_vis_points',label:'Points'},
                 ]),
        TextProperty('arc_osc_msg', 'OSC Message Name')
    ]);
var HandWidget = Widget('Hands', 'hands', [
        SelectProperty('hands_visualization', 'Visualization',
                [{id:'hands_vis_none',label:'None'},
                 {id:'hands_vis_neon',label:'Neon'}]),
        TextProperty('hands_osc_msg', 'OSC Message Name')
    ]);
var BallWidget = Widget('Ball', 'ball', [
        SelectProperty('ball_visualization', 'Visualization',
                [{id:'ball_vis_none',label:'None'},
                 {id:'ball_vis_line',label:'Line'},
                 {id:'ball_vis_lightning',label:'Lightning'},
                 {id:'ball_vis_ball',label:'Ball'},
                 ]),
        TextProperty('ball_osc_msg', 'OSC Message Name')
    ]);
var BodyWidget = Widget('Body', 'body', [
        SelectProperty('body_visualization', 'Visualization',
                [{id:'body_vis_none',label:'None'},
                 {id:'body_vis_line',label:'Line'},
                 ]),
        TextProperty('body_osc_msg', 'OSC Message Name')
    ]);
var WaveWidget = Widget('Wave', 'wave', [
        SelectProperty('wave_visualization', 'Visualization',
                [{id:'wave_vis_none',label:'None'},
                 {id:'wave_vis_knob',label:'Knob'},
                 {id:'wave_vis_pie',label:'Pie'},
                 ]),
        TextProperty('wave_osc_msg', 'OSC Message Name')
    ]);
var PunchWidget = Widget('Punch', 'punch', [
        SelectProperty('punch_visualization', 'Visualization',
                [{id:'punch_vis_none',label:'None'},
                 {id:'punch_vis_flash',label:'Flash'},
                 {id:'punch_vis_pow',label:'Pow'},
                 ]),
        TextProperty('punch_osc_msg', 'OSC Message Name')
    ]);
var ClapWidget = Widget('Clap', 'clap', [
        SelectProperty('clap_visualization', 'Visualization',
                [{id:'clap_vis_none',label:'None'},
                 {id:'clap_vis_flash',label:'Flash'},
                 {id:'clap_vis_pow',label:'Pow'},
                 ]),
        TextProperty('clap_osc_msg', 'OSC Message Name')
    ]);
function showColor2(e) {
    if ($("#bg_style_gradient").attr('checked')) $("#bg_style_color1").show();
    else $("#bg_style_color1").hide();
}
var saveColors = function() {
    // HACK: Click on the save kiblet button
    $("button.kiblet_save").click();
}

var BgWidget = Widget('Background', 'background', [
        SelectProperty('avatar_style_picker', 'Avatar Style', 
                [{id:'avatar_style_img',label:'Actual Image'},
                 {id:'avatar_style_silhouette',label:'Silhouette'},
                 {id:'avatar_style_skeleton',label:'Skeleton'},
                 {id:'avatar_style_polygon',label:'Polygons'}]),
        SelectProperty('bg_style_picker', 'Background Style', 
                [{id:'bg_style_gradient',label:'Gradient',change:showColor2},
                 {id:'bg_style_solid',label:'Solid Color',change:showColor2},
                 {id:'bg_style_stars',label:'Starfield',change:showColor2},
                 {id:'bg_style_psychedelic',label:'Psychedelic',change:showColor2}]),
        ColorProperty('bg_style_color', 'Background Color', '#ff0000', saveColors, 2),
        ColorProperty('fg_style_color', 'Foreground Color', '#ffffff', saveColors),
    ], 'default');
var KibletWidget = Widget('Kiblet', 'kiblet', [
        TextProperty('kiblet_name', 'Kiblet Name', function() {
                                                       var id = $("input[name=timelinekiblets]:checked").attr('id');
                                                       $('#' + id + '_label').html($('#kiblet_name').val());
                                                   }
        ),
        SelectProperty('kiblet_trigger', 'Trigger',
            [{id:'kiblet_trigger_time',label:'Time',change:function(e) {
                                                        $('#kiblet_trigger_info_label').html("Start Time (mm:ss)");
                                                    }
            },
            {id:'kiblet_trigger_kibevent',label:'KIB Event',change:function(e) {
                                                                  $('#kiblet_trigger_info_label').html("KIB OSC Event Name");
                                                                
                                                             }
            },
             {id:'kiblet_trigger_oscevent',label:'Custom OSC Event',change:function(e) {
                                                                        $('#kiblet_trigger_info_label').html("OSC Event Name");
                                                                    }
             }]),
        TextProperty('kiblet_trigger_info', 'Start Time (mm:ss)'),
        {
             update: function() { 
                 $('button.kiblet_save').button();
             },
             getHtml: function() { 
                 return "<div class='setting'><button class='kiblet_save'>Save Kiblet</button></div>";
             },
             getValue: function() { return null; },
             id: "save_kiblet" 
        },
    ], 'default');
