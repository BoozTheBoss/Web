import { json } from '@sveltejs/kit';
import fs from 'fs';
import multer from 'multer'

const upload = multer({ dest: 'static/audio/' });

/** @type {import('./$types').RequestHandler} */
// export async function POST({ request }) {

//   console.log(request);

//   let fileData = Buffer.alloc(0);

//   request.on('data', (chunk) => {
//     fileData = Buffer.concat([fileData, chunk]);
//   });

//   request.on('end', () => {
//     const filename = request.headers['x-filename'];

//     // write the file to disk
//     const newFilePath = 'static/audio/' + filename;
//     fs.writeFile(newFilePath, fileData, (err) => {
//       if (err) {
//         console.error(err);
//       }
//         // res.writeHead(500);
//         // res.end('Error writing file');
//       // } else {
//       //   res.writeHead(200);
//       //   res.end('File uploaded successfully');
//       // }
//     });
//   });



//   // const data = await request.formData();
//   // const file = data.get("audio");
//   // file.write(`static/audio/${file.name}`)
//   // fs.writeFileSync(`static/audio/${file.name}`, new Buffer(file));
//   return json({ ok: 200 });
// }

/** @type {import('./$types').RequestHandler} */
export async function GET({ request }) {
  const lsdir = fs.readdirSync('static/audio');
  return json({ ok: 200, dir: lsdir });
}