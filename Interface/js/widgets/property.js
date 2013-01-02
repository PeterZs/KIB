var SliderProperty = function(prefix, lbl, min, max, onslide) {
    var id = prefix;
    var label = lbl;
    var that = {
        update: function(v) {
            $('#' + id).slider({
                value:(v>=min && v <= max)?v:min,
                min:min,
                max:max,
                step:1,
                slide: onslide
            });
        },
        getHtml: function() {
            return "<div class='setting'>" + label + "<div id='" + id + "' width=200px>" +
		        "</div><span id='" + id + "_label'>" + min + "</span></div>";
        },
        getValue: function() {
            return $('#' + id).slider("value");
        },
        id: id,
    };
    return that;
};
var TextProperty = function(prefix, lbl, change) {
    var id = prefix;
    var label = lbl;
    var that = {
        update: function(v) {
            $("#" + id).val(v);
            if (change) $("#" + id).change(change);
        },
        getHtml: function() {
            return "<div class='setting'><label id='" + id + "_label' for='" + id + "'>" + label + "</label><br><input type='text' id='" + id + "' /></div>";
        },
        getValue: function() {
            return $("#" + id).val();
        },
        id: id,
    };
    return that;
};
var ColorProperty = function(prefix, lbl, change) {
    var id = prefix;
    var label = lbl;
    var that = {
        update: function(v) {
            $.farbtastic('#' + id,change?change:null).setColor(v?v:"#000000");
        },
        getHtml: function() {
            return "<div class='setting'>" + label + "<br><div id='" + id + "'></div></div>";
        },
        getValue: function() {
            return $.farbtastic('#' + id).color;
        },
        id: id,
    };
    return that;
};
var SelectProperty = function(prefix, lbl, choices) {
    var id = prefix;
    var label = lbl;
    var that = {
        update: function(v) {
            if (v) $("#" + v).attr('checked', 'checked');
            $("#" + id).buttonset();
            for (var i = 0; i < choices.length; ++i) {
                if (choices[i].change) {
                    $('#' + choices[i].id).change({id:id}, choices[i].change);
                }
            }
        },
        getHtml: function() {
            var ret = "<div class='setting'>" + label + "<br><span id='" + id + "'>";
            for (var i = 0; i < choices.length; ++i) {
                ret += "<input type='radio' " + (i?'':"checked ") + "id='" + choices[i].id + "' name='" + id + "_radio'/><label id = '" + choices[i].id + "_label' for='" + choices[i].id + "'>" + choices[i].label + "</label>";
            }
            ret += "</span></div>";
            return ret;
        },
        getValue: function() {
            return $('input[name=' + id + '_radio]:checked').attr('id');
        },
        id: id,
    };
    return that;
};