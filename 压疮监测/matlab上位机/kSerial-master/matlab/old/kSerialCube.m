% 
%       __            ____
%      / /__ _  __   / __/                      __  
%     / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
%    / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
%   /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
%                     /_/   github.com/KitSprout    
%  
%  @file    kSerialCube.m
%  @author  KitSprout
%  @date    02-Jul-2017
%  @brief   
% 

classdef kSerialCube < handle

properties (SetAccess = public)
    origin;
    scale;
    color;
    rotate;
    qText;
    attitude;
end

properties (SetAccess = private)
    fig;
    coordinate;
    vertices;
    face;
    surface;
    window;
end

methods

    function cube = kSerialCube( origin, scale, window )
        cube.origin = [ origin(1), origin(2), origin(3) ];
        cube.scale  = [ scale(1), scale(2), scale(3) ];
        cube.color  = [ 'r', 'g', 'b', 'y', 'm', 'c'];
        cube.rotate = [ 1, 0, 0; 0, 1, 0; 0, 0, 1 ];
        cube.attitude = [ 0; 0; 0];
        cube.coordinate = [  1,  1, -1; -1,  1, -1; -1,  1,  1;  1,  1,  1;
                            -1, -1,  1;  1, -1,  1;  1, -1, -1; -1, -1, -1 ];
        cube.coordinate = 0.5 * cube.coordinate;
        cube.vertices = [ cube.coordinate(:, 1) * cube.scale(1), cube.coordinate(:, 2) * cube.scale(2), cube.coordinate(:, 3) * cube.scale(3) ];
        cube.face = [ 1, 2, 3, 4; 4, 3, 5, 6; 6, 7, 8, 5;
                      1, 2, 8, 7; 6, 7, 1, 4; 2, 3, 5, 8 ];
        cube.window = [-window(1), window(1), -window(2), window(2), -window(3), window(3)];
    end

	function initCube( cube, fig, v )
        cube.fig = fig;
        grid(cube.fig, 'on');
        hold(cube.fig, 'on');
        axis(cube.fig, 'equal');
        axis(cube.fig, cube.window);
        xlabel(cube.fig, 'x-axis');
        ylabel(cube.fig, 'y-axis');
        zlabel(cube.fig, 'z-axis');
        cube.surface(1) = patch(cube.fig, 'Faces', cube.face(5, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(1));     % + x
        cube.surface(2) = patch(cube.fig, 'Faces', cube.face(1, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(2));     % + y
        cube.surface(3) = patch(cube.fig, 'Faces', cube.face(2, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(3));     % + z
        cube.surface(4) = patch(cube.fig, 'Faces', cube.face(6, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(4));     % - x
        cube.surface(5) = patch(cube.fig, 'Faces', cube.face(3, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(5));     % - y
        cube.surface(6) = patch(cube.fig, 'Faces', cube.face(4, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(6));     % - z
        cube.qText = text(cube.fig, cube.window(4) + 1, cube.window(1) - 1, 0, '', 'HorizontalAlignment', 'left');
        view(v);
    end

    function plotCube( cube, p, q )
        cube.origin = p;
        cube.vertices = [ cube.coordinate(:, 1) * cube.scale(1), cube.coordinate(:, 2) * cube.scale(2), cube.coordinate(:, 3) * cube.scale(3) ];
        cube.rotate(1, 1) = 2.0 * (q(1) * q(1) + q(2) * q(2) - 0.5);
        cube.rotate(2, 1) = 2.0 * (q(2) * q(3) - q(1) * q(4));
        cube.rotate(3, 1) = 2.0 * (q(2) * q(4) + q(1) * q(3));
        cube.rotate(1, 2) = 2.0 * (q(2) * q(3) + q(1) * q(4));
        cube.rotate(2, 2) = 2.0 * (q(1) * q(1) + q(3) * q(3) - 0.5);
        cube.rotate(3, 2) = 2.0 * (q(3) * q(4) - q(1) * q(2));
        cube.rotate(1, 3) = 2.0 * (q(2) * q(4) - q(1) * q(3));
        cube.rotate(2, 3) = 2.0 * (q(3) * q(4) + q(1) * q(2));
        cube.rotate(3, 3) = 2.0 * (q(1) * q(1) + q(4) * q(4) - 0.5);
        cube.vertices = cube.rotate' * cube.vertices';
        cube.vertices = cube.vertices';
        cube.vertices = [ cube.vertices(:, 1) + cube.origin(1), cube.vertices(:, 2) + cube.origin(2), cube.vertices(:, 3) + cube.origin(3) ];

        delete(cube.surface);
        cube.surface(1) = patch(cube.fig, 'Faces', cube.face(5, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(1));     % + x
        cube.surface(2) = patch(cube.fig, 'Faces', cube.face(1, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(2));     % + y
        cube.surface(3) = patch(cube.fig, 'Faces', cube.face(2, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(3));     % + z
        cube.surface(4) = patch(cube.fig, 'Faces', cube.face(6, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(4));     % - x
        cube.surface(5) = patch(cube.fig, 'Faces', cube.face(3, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(5));     % - y
        cube.surface(6) = patch(cube.fig, 'Faces', cube.face(4, :), 'Vertices', cube.vertices, 'FaceColor', cube.color(6));     % - z
        drawnow
    end

    function att = getAttitude( cube, q )
        r11 = 2.0 * (q(1) * q(1) + q(2) * q(2) - 0.5);
        r12 = 2.0 * (q(2) * q(3) + q(1) * q(4));
        r13 = 2.0 * (q(2) * q(4) - q(1) * q(3));
        r23 = 2.0 * (q(3) * q(4) + q(1) * q(2));
        r33 = 2.0 * (q(1) * q(1) + q(4) * q(4) - 0.5);
        att = [ asin(r13); atan2(-r23, r33); atan2(r12, r11) ] * 180 / pi;
        cube.attitude = att;
    end

end

end
