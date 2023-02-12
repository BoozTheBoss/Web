
/** @type {import('./$types').RequestHandler} */


// api/routes/upload.js
export default function(req, res) {

    if (req.method === 'GET') {
        const files = getFiles();
        res.status(200).json(files);
      } else {
        res.status(405).end();
      }

    if (req.method === 'POST') {
      const file = req.body.file;
  
      // Store the file, for example, in a database or file system
      const fileId = storeFile(file);
  
      res.status(200).json({ fileId, fileName: file.name });
    } else {
      res.status(405).end();
    }
}