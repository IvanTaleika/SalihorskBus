#ifndef UTILS_H
#define UTILS_H

#endif // UTILS_H

const QString POGRAM_DESCRIPTION = "SalihorskBus является заданием по курсовому проектированию по предмету КПиЯП."
                                   " Использование в коммерческих целях запрещено. Расписание взято с сайта %1, все иконки взяты с сайта %2."
                                   " Информация о среде разработки предоставлена во вкладке \"О Qt\".\nВерсия приложения: %3.%4";

const QString SCHEDULE_SOURCE = "https://rassol.by/";

const QString ICON_SOURCE = "https://www.flaticon.com/";

const QString GET_UPDATE_DATE_JS = "$('meta[name=Description]').attr('content').replace(\"Последнее обновление \", \"\")";

const QString GET_BUSES_LINKS = " \
        function getBuses() { \
            var result = []; \
            var busesLinks = $('.col-gutter'); \
            for (var i = 0; i < busesLinks.length; i++) { \
                result.push($(busesLinks[i]).find('a').attr('href')); \
            } \
            return result; \
        }; getBuses();";

const QString GET_BUS_INFORMATION = " \
        function splitTimetable(timetable) {  \
            var result = []; \
            var times = $(timetable).find('[class^=\\\"time\\\"]');\
            for(var i = 0; i < times.length; i++) {  \
                result.push($(times[i]).text());  \
            } \
            return result;  \
        } \
        function getDayTypes(station) {  \
            var result = [];  \
            var dayTypeTimetable = $(station).find('[class=\\\"dir-col-section-item no-list\\\"]'); \
            if (dayTypeTimetable.length == 0) { \
                var dayTypes = $(station).find('[class=\\\"dir-col-section-item\\\"]').find('[for^=\\\"time-work\\\"]'); \
                var dayTypeTimetable = $(station).find('[class=\\\"block-rasp\\\"]'); \
                for(var i = 0; i < dayTypes.length; i++) {  \
                    map = { \
                        dayType: $(dayTypes[i]).text(),  \
                        timetable: splitTimetable(dayTypeTimetable[i]) \
                    };  \
                    result.push(map);  \
                } \
            } else { \
                var dayTypes = $(station).find('[class=\\\"dir-col-section-item\\\"]'); \
                for(var i = 0; i < dayTypes.length; i++) {  \
                    map = { \
                        dayType: $(dayTypes[i]).text(),  \
                        timetable: splitTimetable(dayTypeTimetable[i]) \
                    };  \
                    result.push(map);  \
                } \
            }\
            return result;  \
        } \
        function getStationItem(stations) {  \
            var result = [];  \
            for(var i = 0; i < stations.length; i++) {  \
                map = { \
                    station: $(stations[i]).find('[class=\\\"dir-col-section-title\\\"]').text(),  \
                    dayTypes: getDayTypes(stations[i]) \
                };  \
                result.push(map);  \
            } \
            return result;  \
        }  \
        function getNotes() {  \
            var result = [];  \
            var notes = $('.content').find('ul:has(p)').find('li'); \
            for(var i = 0; i < notes.length; i++) {  \
                result.push($(notes[i]).text());  \
            } \
            return result;  \
        }  \
        function getBusDirections() {  \
          var result = [];  \
          var directions = $('span[id^=\\\"napr\\\"]');  \
          var directionsInfo = $('div[id^=\\\"napr\\\"]');  \
          for (var i = 0; i < directions.length; i++) {  \
              var map = {  \
                    direction: $(directions[i]).text(),  \
                    stations: getStationItem($(directionsInfo[i]).find('[class=\\\"dir-col-section\\\"]'))  \
              };  \
              result.push(map);  \
          }  \
          return result;  \
        };  \
        function getBusInformation() {  \
          var result = {  \
              number: /№(.*)/.exec($('h1[class=\\\"title-imp\\\"]').text())[1],  \
              directions: getBusDirections(),  \
              notes: getNotes() \
          };  \
          return result;  \
        };  \
        getBusInformation();";


