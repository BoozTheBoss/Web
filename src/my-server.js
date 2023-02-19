import { handler } from './build/handler.js';
import express from 'express';
import multer from 'multer';
 
const app = express();

const upload = multer();

// chatGPT
app.post('/upload', upload.single('file'), (req, res) => {
  const file = req.file;
  const fileData = Buffer.from(file.buffer, 'binary');

  // write the file to disk
  const newFilePath = 'static/audio/' + file.originalname;
  fs.writeFile(newFilePath, fileData, (err) => {
    if (err) {
      console.error(err);
      res.sendStatus(500);
    } else {
      res.json({ message: 'File uploaded successfully' });
    }
  });
});

// add a route that lives separately from the SvelteKit app
app.get('/healthcheck', (req, res) => {
  res.end('ok');
});
 
// let SvelteKit handle everything else, including serving prerendered pages and static assets
app.use(handler);
 
app.listen(3000, () => {
  console.log('listening on port 3000');
});