var Skeleton = function() {
    var joints = {
        head: [0, 0.7],
        hips: [0, 0.35],
        rightfoot: [-0.3, 0.01],
        leftfoot:  [ 0.3, 0.01],
        righthand: [-0.35, 0.3],
        lefthand:  [ 0.35, 0.3],
        rightshoulder: [-0.25, 0.5],
        leftshoulder:  [ 0.25, 0.5],
    };
    // Joint locations: Lhand, Rhand, Lshoulder, Rshoulder, Head, Hip
    var that = {
        update: function() {
            // Set random positions for things  
        },
        init: function() {
            // Add mouse handlers to drag arms and head
        },
        joint: function(which, w, h) {
            if (!w) w = 0;
            if (!h) h = 0;
            var m = w < h?w:h;
            return [w/2 + m*joints[which][0]/2, h - m*joints[which][1]];
        },
    };
    return that;
};

function render(kiblet, skeleton) {
    var widgets = ['background', 'arc', 'body', 'ball', 'wave', 'hands', 'punch', 'clap'];
    var render = [renderBackground, renderArc, renderBody, renderBall, renderWave, renderHands, renderPunch, renderClap];

    var ctx = $('#visualization').get(0);
    
    for (var i = 0; i < widgets.length; ++i) {
        if (widgets[i] in kiblet) {
            render[i](kiblet[widgets[i]], skeleton, ctx.getContext("2d"), ctx.width, ctx.height);
        }
    }
}

