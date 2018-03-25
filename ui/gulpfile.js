var gulp = require('gulp');
var browserify = require('browserify');
var babelify = require('babelify');
var source = require('vinyl-source-stream');
var uglify = require('gulp-uglify');
var rename = require('gulp-rename');
var buffer = require('vinyl-buffer');

gulp.task('build', function () {
    return browserify({entries: './app.jsx', extensions: ['.jsx'], debug: true})
        .transform('babelify', {presets: ['es2015', 'react'], plugins: ['transform-object-rest-spread']})
        .bundle()
        .on('error', function(err) { console.error(err); this.emit('end'); })
        .pipe(source('bundle.js'))
        //.pipe(buffer()) // uncomment these lines to minify
        //.pipe(uglify())
        //.pipe(rename({suffix: '.min'}))
        .pipe(gulp.dest('dist'));
});

gulp.task('watch', ['build'], function () {
    gulp.watch(['./*.jsx','C:/Users/u0078867/Desktop/KULeuvenJob/my_node_modules/react-fast-charts-exp/*.*','C:/Users/u0078867/Desktop/KULeuvenJob/my_node_modules/common-utils/*.*'], ['build']);
});

gulp.task('default', ['watch']);
