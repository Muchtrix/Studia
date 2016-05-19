function Init() {
  // Parametry gry i referencja do planszy
   document.getElementById("time").innerHTML = 0;
   var pa = document.getElementById("playArea");
   pa.innerHTML = "";
   columns = 16;
   rows = 16;
   mines = 40;
   flags = 0;
   gameStart = true;
   timeInt = null;
   startTime = new Date();
   gameGoing = true;
   finalShowing = false;
   freeSpaces = rows*columns - mines;
   document.getElementById("mines").innerHTML = "Miny: " + mines;
   document.getElementById("message").innerHTML = "Wolne pola: "+ freeSpaces + "<br/><br/>";

  // Tablica odkrytych pól i utworzenie grafik
   odkryte = [];
   oflagowane = [];
  for (var i = 0; i < rows; i++){
    for (var j = 0; j < columns; j++){
      var kafelek = document.createElement("img");
      kafelek.src = "assets/empty.png";
      kafelek.alt = "Pole gry";
      var ile = i*columns + j;
      kafelek.id = ile;
      kafelek.onclick = function() {klikniecie(this)};
      kafelek.oncontextmenu = function(){flaga(this); return false};
      pa.appendChild(kafelek);
      odkryte.push(false);
      oflagowane.push(false);
    }
    pa.appendChild(document.createElement("br"));
  }

  // Generacja min na planszy
   miny = [];
  for (var i = 0; i < mines; i++){
    var y = getRandom(0, rows);
    var x = getRandom(0, columns);
    var cord = y*columns + x;
    if (miny.indexOf(cord) < 0) miny.push(cord);
    else i--;
  }
}

// Aktualizacja zegara
function timeUpdate(){
  var d = document.getElementById("time");
  var now = new Date();
  var czas = Math.floor((now.getTime() - startTime.getTime())/ 1000);
  d.innerHTML = czas;
}

// Sasiedzi danego pola na planszy
function sasiedzi(cord) {
  var wyn = [];
  var x = cord%columns;
  var y = Math.floor(cord/columns);
  if(x > 0){             wyn.push(cord-1);
    if(y > 0)            wyn.push(cord-1-columns);
    if(y < rows-1)       wyn.push(cord-1+columns);}
  if(x < columns-1){     wyn.push(cord+1);
    if(y > 0)            wyn.push(cord+1-columns);
    if(y < rows-1)       wyn.push(cord+1+columns);}
  if(y > 0)              wyn.push(cord-columns);
  if(y < rows-1)         wyn.push(cord+columns);
  return wyn;
}

// Obsługa flagowania
function flaga(v){
  
  if (!gameGoing) return;

  var cord = parseInt(v.id);

  // Tryb flagowania
  if (oflagowane[cord]) {
    v.src = "assets/empty.png";
    flags--;
  }else{
    v.src = "assets/flag.png";
    flags++;
  }
  oflagowane[cord] = !oflagowane[cord];
  var ile = flags <= mines ? mines - flags : 0;
  document.getElementById("mines").innerHTML = "Miny: " + ile;
  return;
}

// Obsługa klikniecia w pole
function klikniecie(v) {
  
    if(gameStart){
    startTime = new Date();
    timeInt = setInterval(function(){timeUpdate()}, 500);
    gameStart = false;
  }
  
  // Sprawdzenie czy trwa rozgrywka
  if (!gameGoing) return;

  var cord = parseInt(v.id);

  if (!odkryte[cord] && !oflagowane[cord]){
    odkryte[cord] = true;

    // Trafienie miny
    if (miny.indexOf(cord) >= 0){
      v.src = "assets/bomb.png";
      if (!finalShowing) gameOver();
      return;
    }

    //Zaznaczenie odkrytego pola
    freeSpaces--;
    // Sprawdzenie przylegajacych
    var sas = sasiedzi(cord);
    var ile = 0;
    for (var i = 0; i < sas.length; i++){
      if (miny.indexOf(sas[i]) >= 0) ile++;
    }
    v.src = "assets/"+ile+".png";

    // Jesli odkryto wszystkie wolne pola - koniec gry
    if (freeSpaces == 0) {
      clearInterval(timeInt);
      document.getElementById("message").innerHTML = "Koniec Gry, Gratulacje!";
      gameGoing = false;
    }
    else document.getElementById("message").innerHTML = "Wolne pola: "+ freeSpaces + "<br/><br/>";

    // Jesli nie ma przylegajacych - odkrycie sasiadow
    if (ile == 0)
      for (var i = 0; i < sas.length; i++)
        klikniecie(document.getElementById(sas[i]));
  }
}

// Koniec gry - przegrana
function gameOver(){
  finalShowing = true;
  var ile = columns*rows;
  for (var i = 0; i < ile; i++){
    oflagowane[i] = false;
    klikniecie(document.getElementById(i));
  }
  clearInterval(timeInt);
  document.getElementById("message").innerHTML = "Przegrana!<br/><br/>";
  gameGoing = false;
}

// Generacja liczb losowych z przedziału [min, max)
function getRandom(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}
