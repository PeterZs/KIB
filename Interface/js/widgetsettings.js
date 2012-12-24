var ORIGINAL_CELL_HEIGHT = 0;
function refreshAccordion() {
    var wsc = $('#widgetsettingscontainer');
    var state = wsc.accordion("option", "active");
    wsc.accordion('destroy').accordion();
    wsc.accordion('option', 'active', state);
    var buttonsheight = $('#widgetpicker').outerHeight() + $('#savebuttons').outerHeight();
    var hdrs = wsc.children('h3');
    var numFrames = hdrs.length;
    var headersheight = Math.max(hdrs.outerHeight(), hdrs.last().outerHeight());
    var padding = parseInt($('.widgetsettings').css('padding-top')) + parseInt($('.widgetsettings').css('padding-bottom')) + 30;
    var newheight = ORIGINAL_CELL_HEIGHT - buttonsheight - numFrames*headersheight - padding;
    $('.widgetsettings').height(newheight);
}

function initWidgetSettings(widget, data) {
    var container = $('#' + widget.containerId);
    if (container.length == 0) {
        if (widget.type == 'widget') {
            $('#widgetsettingscontainer').append("<h3 class='widgetsettingheader' id='" + widget.containerId + "hdr'>" + widget.title + "</h3>");
            $('#widgetsettingscontainer').append(widget.settingsstr);
        } else if (widget.type == 'default') {
            $('#widgetsettingscontainer').prepend(widget.settingsstr);
            $('#widgetsettingscontainer').prepend("<h3 class='widgetsettingheader' id='" + widget.containerId + "hdr'>" + widget.title + "</h3>");
        }
        widget.activate();
    }
    // Refresh Accordion
    refreshAccordion();
    widget.loadSettings(data);
}

function removeWidgetSettings(widget, data) {
    var container = $('#' + widget.containerId);
    container.remove();
    $('#' + widget.containerId + 'hdr').remove();
    refreshAccordion();
}

function initWidget(id, info) {
    var i = id + 1;
    $('#widgetpicker').append('<input type="checkbox" id="widget' + i + '"><label for="widget' + i + '"></label>');
    $('#widget'+i).button({icons: {primary: "icon_"+i}});
    $('#widget'+i).change(function() {
        if (this.checked) {
            initWidgetSettings(info.widgets[id], info.data);
        } else {
            removeWidgetSettings(info.widgets[id], info.data);
        }
    });
}
function initPage() {
    // Initialize data and widget objects
    var widgets = [
       ArcWidget(), HandWidget(), ArcWidget(), HandWidget(), ArcWidget(), HandWidget(),
       ArcWidget(), HandWidget(), BgWidget(), KibletWidget()
    ];
    var data = {};
    var ret = {widgets: widgets, data: data};
    ORIGINAL_CELL_HEIGHT = $('#widgetsettingscontainer').parents('td').first().height();

    // Initialize widget buttons
	for (var i = 0; i < 8; ++i) {
        initWidget(i, ret);
	}
    // Initialize buttons
	$('button.kiblet_save').button();
	$('button.kiblet_new').button();
	$('button.ki_save').button();
	$('#widgetsettingscontainer').accordion();
    initWidgetSettings(widgets[8]);
    initWidgetSettings(widgets[9]);
    return ret;
}

function downloadfile(str) {
    var uricontent = "data:text/plain," + encodeURIComponent(str);
    var dl = window.open(uricontent, "KIB Data");
}
