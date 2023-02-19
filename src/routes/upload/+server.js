import { json } from '@sveltejs/kit';
import fs from 'fs';

/** @type {import('./$types').RequestHandler} */
export async function POST({ request }) {
  const formData = await request.formData();
  const file = formData.get('audio');

  const filename = file.name;
  const arraybuffer = await file.arrayBuffer();
  const buffer = Buffer.from(arraybuffer);

  fs.writeFileSync(`static/audio/${filename}`, buffer);
  return json({ ok: 200 });
}

/** @type {import('./$types').RequestHandler} */
export async function GET({ request }) {
  const lsdir = fs.readdirSync('static/audio');
  return json({ ok: 200, dir: lsdir });
}